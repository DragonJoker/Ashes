#include "Pipeline/GlPipeline.hpp"

#include "Buffer/GlBuffer.hpp"
#include "Command/Commands/GlBindPipelineCommand.hpp"
#include "Core/GlDevice.hpp"
#include "Core/GlInstance.hpp"
#include "Miscellaneous/GlValidator.hpp"
#include "Pipeline/GlPipelineLayout.hpp"
#include "RenderPass/GlRenderPass.hpp"
#include "Shader/GlShaderModule.hpp"

#include "ashesgl4_api.hpp"

#include <renderer/RendererCommon/Helper/ColourBlendState.hpp>
#include <renderer/RendererCommon/Helper/DepthStencilState.hpp>
#include <renderer/RendererCommon/Helper/DynamicState.hpp>
#include <renderer/RendererCommon/Helper/InputAssemblyState.hpp>
#include <renderer/RendererCommon/Helper/MultisampleState.hpp>
#include <renderer/RendererCommon/Helper/RasterisationState.hpp>
#include <renderer/RendererCommon/Helper/TessellationState.hpp>
#include <renderer/RendererCommon/Helper/VertexInputState.hpp>
#include <renderer/RendererCommon/Helper/ViewportState.hpp>

#include <ashes/common/Hash.hpp>

#include <algorithm>

#if defined( interface )
#	undef interface
#endif

namespace ashes::gl4
{
	namespace
	{
		size_t doHash( VboBindings const & vbos
			, IboBinding const & ibo )
		{
			size_t result{ 0u };

			for ( auto & binding : vbos )
			{
				auto & vbo = binding.second;
				hashCombine( result, vbo.bo );
				hashCombine( result, vbo.buffer );
				hashCombine( result, vbo.offset );
			}

			if ( bool( ibo ) )
			{
				hashCombine( result, ibo.value().bo );
				hashCombine( result, ibo.value().buffer );
				hashCombine( result, ibo.value().offset );
			}

			return result;
		}

		size_t doHash( VkVertexInputAttributeDescription const & desc )
		{
			size_t result = 0u;
			hashCombine( result, desc.binding );
			hashCombine( result, desc.format );
			hashCombine( result, desc.location );
			hashCombine( result, desc.offset );
			return result;
		}

		size_t doHash( VkVertexInputBindingDescription const & desc )
		{
			size_t result = 0u;
			hashCombine( result, desc.binding );
			hashCombine( result, desc.inputRate );
			hashCombine( result, desc.stride );
			return result;
		}

		size_t doHash( VkPipelineVertexInputStateCreateInfo const & state )
		{
			size_t result = 0u;
			auto const endAttribs = state.pVertexAttributeDescriptions + state.vertexAttributeDescriptionCount;

			for ( auto it = state.pVertexAttributeDescriptions; it != endAttribs; ++it )
			{
				hashCombine( result, doHash( *it ) );
			}

			auto const endBindings = state.pVertexBindingDescriptions + state.vertexBindingDescriptionCount;

			for ( auto it = state.pVertexBindingDescriptions; it != endBindings; ++it )
			{
				hashCombine( result, doHash( *it ) );
			}

			return result;
		}

		Optional< VkPipelineRasterizationStateCreateInfo > invertFrontFace( VkPipelineRasterizationStateCreateInfo const * state )
		{
			if ( state )
			{
				VkPipelineRasterizationStateCreateInfo result{ *state };
				result.frontFace = VkFrontFace( VK_FRONT_FACE_END_RANGE - result.frontFace );
				return result;
			}

			return Optional< VkPipelineRasterizationStateCreateInfo >{};
		}

		InterfaceBlocksLayout merge( InterfaceBlocksLayout const & lhs
			, InterfaceBlocksLayout const & rhs )
		{
			InterfaceBlocksLayout result{ lhs };

			if ( !rhs.empty() )
			{
				if ( result.empty() )
				{
					result = rhs;
				}
				else
				{
					for ( auto & constantBuffer : rhs )
					{
						auto it = std::find_if( result.begin()
							, result.end()
							, [&constantBuffer]( ConstantBufferDesc const & lookup )
							{
								return lookup.binding == constantBuffer.binding;
							} );

						if ( it == result.end() )
						{
							result.push_back( constantBuffer );
						}
					}
				}
			}

			std::sort( result.begin()
				, result.end()
				, []( ConstantBufferDesc const & lhs, ConstantBufferDesc const & rhs )
				{
					return lhs.binding < rhs.binding;
				} );

			return result;
		}

		template< typename FormatT >
		DescLayoutT< FormatT > merge( DescLayoutT< FormatT > const & lhs
			, DescLayoutT< FormatT > const & rhs )
		{
			DescLayoutT< FormatT > result{ lhs };

			if ( !rhs.empty() )
			{
				if ( result.empty() )
				{
					result = rhs;
				}
				else
				{
					for ( auto & constant : rhs )
					{
						auto it = std::find_if( result.begin()
							, result.end()
							, [&constant]( FormatDescT< FormatT > const & lookup )
							{
								return lookup.name == constant.name
									&& lookup.program == constant.program;
							} );

						if ( it == result.end() )
						{
							result.push_back( constant );
						}
					}
				}
			}

			std::sort( result.begin()
				, result.end()
				, []( FormatDescT< FormatT > const & lhs, FormatDescT< FormatT > const & rhs )
				{
					return lhs.offset < rhs.offset;
				} );

			return result;
		}

		ShaderDesc merge( std::vector< ShaderDesc > const & descs )
		{
			ShaderDesc result{};

			for ( auto & desc : descs )
			{
				result.isGlsl = result.isGlsl || desc.isGlsl;
				result.inputs.vertexAttributeDescriptions.insert( result.inputs.vertexAttributeDescriptions.end()
					, desc.inputs.vertexAttributeDescriptions.begin()
					, desc.inputs.vertexAttributeDescriptions.end() );
				result.inputs.vertexBindingDescriptions.insert( result.inputs.vertexBindingDescriptions.end()
					, desc.inputs.vertexBindingDescriptions.begin()
					, desc.inputs.vertexBindingDescriptions.end() );
				result.stageFlags |= desc.stageFlags;
				result.pcb = merge( result.pcb
					, desc.pcb );
				result.ubo = merge( result.ubo
					, desc.ubo );
				result.sbo = merge( result.sbo
					, desc.sbo );
				result.img = merge( result.img
					, desc.img );
				result.tex = merge( result.tex
					, desc.tex );
			}

			return result;
		}

		uint32_t getBinding( ConstantBufferDesc const & value )
		{
			return value.binding;
		}

		template< typename FormatT >
		uint32_t getBinding( FormatDescT< FormatT > const & value )
		{
			return value.location;
		}

		bool checkDesc( VkWriteDescriptorSet const & write
			, ConstantBufferDesc const & lookup )
		{
			return false;
		}

		template< typename FormatT >
		bool checkDesc( VkWriteDescriptorSet const & write
			, FormatDescT< FormatT > const & lookup )
		{
			return false;
		}

		template< typename DescT >
		DescT const * findDesc( VkWriteDescriptorSet const & write
			, uint32_t descriptorSetIndex
			, std::vector< DescT > const & descLayout
			, ShaderBindingMap const & bindings
			, ShaderBindingMap::const_iterator & bindingIt )
		{
			bindingIt = bindings.end();
			auto it = std::find_if( descLayout.begin()
				, descLayout.end()
				, [&write, &descriptorSetIndex, &bindings, &bindingIt]( DescT const & lookup )
				{
					bool result = write.dstBinding == getBinding( lookup )
						|| checkDesc( write, lookup );

					if ( result )
					{
						bindingIt = bindings.find( makeShaderBindingKey( descriptorSetIndex, write.dstBinding ) );
					}

					return result;
				} );

			if ( it == descLayout.end() )
			{
				return nullptr;
			}

			return &( *it );
		}

		VkDescriptorSetLayoutBinding convert( VkDescriptorType descriptorType
			, uint32_t descriptorCount
			, uint32_t binding )
		{
			return
			{
				binding,
				descriptorType, // descriptorType
				descriptorCount, // descriptorCount
				0u, // stageFlags
				nullptr, // pImmutableSamplers
			};
		}

		template< typename DescContT >
		void doReworkWrites( uint32_t descriptorSetIndex
			, LayoutBindingWritesArray const & writesArray
			, DescContT const & descs
			, ShaderBindingMap & resultMap
			, ShaderBindings & result )
		{
			for ( auto & array : writesArray )
			{
				for ( auto & write : array->writes )
				{
					ShaderBindingMap::const_iterator bindingIt;
					auto desc = findDesc( write
						, descriptorSetIndex
						, descs
						, resultMap
						, bindingIt );
					addReplaceBinding( descriptorSetIndex
						, write.dstBinding
						, convert( write.descriptorType
							, write.descriptorCount
							, ( desc
								? getBinding( *desc )
								: ( bindingIt != resultMap.end()
									? bindingIt->second
									: write.dstBinding ) ) )
						, result );
				}
			}
		}

		ShaderBindings doReworkBindings( ShaderBindings const & srcBindings
			, VkDescriptorSet descriptorSet
			, uint32_t descriptorSetIndex
			, ShaderDesc const & programLayout )
		{
			ShaderBindings result = srcBindings;

			if ( programLayout.isGlsl )
			{
				doReworkWrites( descriptorSetIndex
					, get( descriptorSet )->getStorageTextures()
					, programLayout.img
					, result.img
					, result );
				doReworkWrites( descriptorSetIndex
					, get( descriptorSet )->getCombinedTextureSamplers()
					, programLayout.tex
					, result.tex
					, result );
				doReworkWrites( descriptorSetIndex
					, get( descriptorSet )->getSamplers()
					, programLayout.tex
					, result.tex
					, result );
				doReworkWrites( descriptorSetIndex
					, get( descriptorSet )->getSampledTextures()
					, programLayout.tex
					, result.tex
					, result );
				doReworkWrites( descriptorSetIndex
					, get( descriptorSet )->getInputAttachments()
					, programLayout.tex
					, result.tex
					, result );
				doReworkWrites( descriptorSetIndex
					, get( descriptorSet )->getStorageBuffers()
					, programLayout.sbo
					, result.sbo
					, result );
				doReworkWrites( descriptorSetIndex
					, get( descriptorSet )->getUniformBuffers()
					, programLayout.ubo
					, result.ubo
					, result );
				doReworkWrites( descriptorSetIndex
					, get( descriptorSet )->getInlineUniforms()
					, programLayout.ubo
					, result.ubo
					, result );
			}

			return result;
		}

		Pipeline::ProgramPipeline createProgramPipeline( VkDevice device
			, ContextState * state
			, VkPipeline pipeline
			, VkPipelineShaderStageCreateInfoArray stages
			, VkPipelineLayout layout
			, VkPipelineCreateFlags createFlags
			, VkRenderPass renderPass
			, Optional< VkPipelineVertexInputStateCreateInfo > const & vertexInputState
			, bool invertY = false )
		{
			auto context = get( device )->getContext();

			if ( state )
			{
				ContextStateStack stack{ device };
				stack.apply( context
					, *state );
			}

			std::vector< ShaderDesc > descs;

			for ( auto & stage : stages )
			{
				descs.push_back( get( stage.module )->compile( stage, layout, createFlags, invertY ) );
			}

			Pipeline::ProgramPipeline result;
			result.program = merge( descs );
			result.bindings = get( layout )->getShaderBindings();
			glLogCall( context
				, glGenProgramPipelines
				, 1
				, &result.program.program );
			glLogCall( context
				, glBindProgramPipeline
				, result.program.program );
			uint32_t index = 0u;

			for ( auto & desc : descs )
			{
				glLogCall( context
					, glUseProgramStages
					, result.program.program
					, convertShaderStageFlag( stages[index].stage )
					, descs[index].program );
				result.modules.push_back( descs[index].program );
				++index;
			}

			glLogCall( context
				, glBindProgramPipeline
				, 0 );

			result.constantsPcb.stageFlags = result.program.stageFlags;
			uint32_t size = 0u;

			for ( auto & constant : result.program.pcb )
			{
				result.constantsPcb.constants.push_back( constant );
				size += constant.size;
			}

			result.constantsPcb.size = size;

			if ( get( get( device )->getInstance() )->isValidationEnabled()
				&& renderPass != VK_NULL_HANDLE )
			{
				validatePipeline( context
					, layout
					, result.program.program
					, vertexInputState.value()
					, renderPass );
			}

			return result;
		}
	}

	Pipeline::Pipeline( VkDevice device
		, VkGraphicsPipelineCreateInfo createInfo )
		: m_device{ device }
		, m_flags{ createInfo.flags }
		, m_stages{ makeVector( createInfo.pStages, createInfo.stageCount ) }
		, m_vertexInputState{ makeOptional( createInfo.pVertexInputState
			, m_vertexBindingDescriptions
			, m_vertexAttributeDescriptions ) }
		, m_backContextState
		{
			*createInfo.pColorBlendState,
			createInfo.pDepthStencilState,
			createInfo.pMultisampleState,
			createInfo.pTessellationState,
			createInfo.pInputAssemblyState,
			createInfo.pViewportState,
			createInfo.pRasterizationState,
			createInfo.pDynamicState,
		}
		, m_rtotRasterizationState{ invertFrontFace( createInfo.pRasterizationState ) }
		, m_rtotContextState
		{
			*createInfo.pColorBlendState,
			createInfo.pDepthStencilState,
			createInfo.pMultisampleState,
			createInfo.pTessellationState,
			createInfo.pInputAssemblyState,
			createInfo.pViewportState,
			( bool( m_rtotRasterizationState )
				? &m_rtotRasterizationState.value()
				: nullptr ),
			createInfo.pDynamicState,
		}
		, m_layout{ createInfo.layout }
		, m_renderPass{ createInfo.renderPass }
		, m_subpass{ createInfo.subpass }
		, m_basePipelineHandle{ createInfo.basePipelineHandle }
		, m_basePipelineIndex{ createInfo.basePipelineIndex }
		, m_backPipeline{ createProgramPipeline( m_device, &m_backContextState, get( this ), m_stages, m_layout, createInfo.flags, m_renderPass, m_vertexInputState, true ) }
		, m_rtotPipeline{ createProgramPipeline( m_device, &m_rtotContextState, get( this ), m_stages, m_layout, createInfo.flags, m_renderPass, m_vertexInputState, false ) }
		, m_vertexInputStateHash{ ( m_vertexInputState
			? doHash( m_vertexInputState.value() )
			: 0u ) }
	{
	}

	Pipeline::Pipeline( VkDevice device
		, VkComputePipelineCreateInfo createInfo )
		: m_device{ device }
		, m_stages{ makeVector( &createInfo.stage, 1u ) }
		, m_layout{ createInfo.layout }
		, m_basePipelineHandle{ createInfo.basePipelineHandle }
		, m_basePipelineIndex{ createInfo.basePipelineIndex }
		, m_compPipeline{ createProgramPipeline( m_device, nullptr, get( this ), m_stages, m_layout, createInfo.flags, m_renderPass, m_vertexInputState ) }
	{
	}

	Pipeline::~Pipeline()
	{
		auto context = get( m_device )->getContext();

		if ( isCompute() )
		{
			glLogCall( context
				, glDeleteProgramPipelines
				, 1u
				, &m_compPipeline.program.program );

			for ( auto & module : m_compPipeline.modules )
			{
				glLogCall( context
					, glDeleteProgram
					, module );
			}
		}
		else
		{
			glLogCall( context
				, glDeleteProgramPipelines
				, 1u
				, &m_backPipeline.program.program );

			for ( auto & module : m_backPipeline.modules )
			{
				glLogCall( context
					, glDeleteProgram
					, module );
			}

			glLogCall( context
				, glDeleteProgramPipelines
				, 1u
				, &m_rtotPipeline.program.program );

			for ( auto & module : m_rtotPipeline.modules )
			{
				glLogCall( context
					, glDeleteProgram
					, module );
			}
		}
	}

	GeometryBuffers * Pipeline::findGeometryBuffers( VboBindings const & vbos
		, IboBinding const & ibo )const
	{
		size_t hash = doHash( vbos, ibo );
		auto it = std::find_if( m_geometryBuffers.begin()
			, m_geometryBuffers.end()
			, [&hash]( std::pair< size_t, GeometryBuffersPtr > const & pair )
			{
				return pair.first == hash;
			} );
		return it == m_geometryBuffers.end()
			? nullptr
			: it->second.get();
	}

	GeometryBuffersRef Pipeline::createGeometryBuffers( VboBindings vbos
		, IboBinding const & ibo
		, VkIndexType type )const
	{
		size_t hash = doHash( vbos, ibo );
		m_geometryBuffers.emplace_back( hash
			, std::make_unique< GeometryBuffers >( m_device
				, vbos
				, ibo
				, m_vertexInputState.value()
				, m_backPipeline.program.inputs
				, type ) );

		for ( auto & binding : vbos )
		{
			auto & vbo = binding.second;
			m_connections.emplace( vbo.bo
				, get( get( vbo.buffer )->getMemory() )->onDestroy.connect( [this]( GLuint name )
					{
						auto it = std::remove_if( m_geometryBuffers.begin()
							, m_geometryBuffers.end()
							, [&name]( std::pair< size_t, GeometryBuffersPtr > const & pair )
							{
								bool result = false;

								for ( auto & vbo : pair.second->getVbos() )
								{
									if ( !result )
									{
										result = get( vbo.vbo )->getInternal() == name;
									}
								}

								if ( !result && bool( pair.second->hasIbo() ) )
								{
									result = get( pair.second->getIbo().ibo )->getInternal() == name;
								}

								return result;
							} );

						m_geometryBuffers.erase( it, m_geometryBuffers.end() );
					} ) );
		}

		return *m_geometryBuffers.back().second;
	}

	PushConstantsDesc Pipeline::findPushConstantBuffer( PushConstantsDesc const & pushConstants
		, bool isRtot )const
	{
		PushConstantsDesc result;

		if ( isCompute() )
		{
			result = m_compPipeline.constantsPcb;
		}
		else if ( isRtot )
		{
			result = m_rtotPipeline.constantsPcb;
		}
		else
		{
			result = m_backPipeline.constantsPcb;
		}

		result.offset = pushConstants.offset;
		result.size = pushConstants.size;
		result.data = pushConstants.data;
		return result;
	}

	VkDescriptorSetLayoutArray const & Pipeline::getDescriptorsLayouts()const
	{
		return get( m_layout )->getDescriptorsLayouts();
	}

	ShaderBindings const & Pipeline::getDescriptorSetBindings( VkDescriptorSet descriptorSet
		, uint32_t descriptorSetIndex )const
	{
		auto key = makeDescriptorKey( descriptorSet, descriptorSetIndex );
		auto pair = m_dsBindings.emplace( key
			, get( m_layout )->getDecriptorSetBindings( descriptorSet, descriptorSetIndex ) );

		if ( pair.second )
		{
			if ( isCompute() )
			{
				pair.first->second = doReworkBindings( pair.first->second
					, descriptorSet
					, descriptorSetIndex
					, m_compPipeline.program );
			}
			else
			{
				pair.first->second = doReworkBindings( pair.first->second
					, descriptorSet
					, descriptorSetIndex
					, m_backPipeline.program );
			}
		}

		return pair.first->second;
	}
}
