#include "Pipeline/GlPipeline.hpp"

#include "Buffer/GlBuffer.hpp"
#include "Command/Commands/GlBindPipelineCommand.hpp"
#include "Core/GlDevice.hpp"
#include "Core/GlInstance.hpp"
#include "Miscellaneous/GlValidator.hpp"
#include "Pipeline/GlPipelineLayout.hpp"
#include "RenderPass/GlRenderPass.hpp"
#include "Shader/GlShaderModule.hpp"

#include "ashesgl3_api.hpp"

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

namespace ashes::gl3
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
		, m_backProgram{ std::make_unique< ShaderProgram >( m_device, get( this ), m_stages, createInfo.flags, false ) }
		, m_rtotProgram{ std::make_unique< ShaderProgram >( m_device, get( this ), m_stages, createInfo.flags, true ) }
		, m_vertexInputStateHash{ ( m_vertexInputState
			? doHash( m_vertexInputState.value() )
			: 0u ) }
	{
		auto context = get( device )->getContext();
		CmdList list;
		ContextStateStack stack{ device };
		{
			stack.apply( context
				, m_backContextState );
			doInitialise( context, *m_backProgram );
		}
		{
			stack.apply( context
				, m_rtotContextState );
			m_rtotProgram->link( context );
		}
	}

	Pipeline::Pipeline( VkDevice device
		, VkComputePipelineCreateInfo createInfo )
		: m_device{ device }
		, m_stages{ makeVector( &createInfo.stage, 1u ) }
		, m_layout{ createInfo.layout }
		, m_basePipelineHandle{ createInfo.basePipelineHandle }
		, m_basePipelineIndex{ createInfo.basePipelineIndex }
		, m_compProgram{ std::make_unique< ShaderProgram >( m_device, get( this ), m_stages.back(), createInfo.flags ) }
	{
		auto context = get( device )->getContext();
		doInitialise( context, *m_compProgram );
	}

	Pipeline::~Pipeline()
	{
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
				, m_shaderDesc.inputLayout
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

	PushConstantsDesc Pipeline::findPushConstantBuffer( PushConstantsDesc const & pushConstants )const
	{
		PushConstantsDesc result{ m_constantsPcb };
		result.offset = pushConstants.offset;
		result.size = pushConstants.size;
		result.data = pushConstants.data;
		return result;
	}

	VkDescriptorSetLayoutArray const & Pipeline::getDescriptorsLayouts()const
	{
		return get( m_layout )->getDescriptorsLayouts();
	}

	void Pipeline::doInitialise( ContextLock const & context
		, ShaderProgram const & program )
	{
		m_shaderDesc = program.link( context );
		m_constantsPcb.stageFlags = m_shaderDesc.stageFlags;
		uint32_t size = 0u;

		for ( auto & constant : m_shaderDesc.constantsLayout )
		{
			m_constantsPcb.constants.push_back( constant );
			size += constant.size;
		}

		m_constantsPcb.size = size;

		if ( get( get( m_device )->getInstance() )->isValidationEnabled() )
		{
			validatePipeline( context
				, m_layout
				, program.getProgram()
				, m_vertexInputState.value()
				, m_renderPass );
		}
	}
}
