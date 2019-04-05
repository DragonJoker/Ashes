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

#include <AshesRenderer/Helper/ColourBlendState.hpp>
#include <AshesRenderer/Helper/DepthStencilState.hpp>
#include <AshesRenderer/Helper/DynamicState.hpp>
#include <AshesRenderer/Helper/InputAssemblyState.hpp>
#include <AshesRenderer/Helper/MultisampleState.hpp>
#include <AshesRenderer/Helper/RasterisationState.hpp>
#include <AshesRenderer/Helper/TessellationState.hpp>
#include <AshesRenderer/Helper/VertexInputState.hpp>
#include <AshesRenderer/Helper/ViewportState.hpp>

#include <algorithm>

#if defined( interface )
#	undef interface
#endif

namespace ashes
{
	inline VkPipelineShaderStageCreateInfo deepCopy( VkPipelineShaderStageCreateInfo const & rhs )
	{
		return rhs;
	}
}

namespace ashes::gl4
{
	namespace
	{
		template<typename T>
		void doHashCombine( size_t & seed, T const & v )
		{
			const uint64_t kMul = 0x9ddfea08eb382d69ULL;

			std::hash< T > hasher;
			uint64_t a = ( hasher( v ) ^ seed ) * kMul;
			a ^= ( a >> 47 );

			uint64_t b = ( seed ^ a ) * kMul;
			b ^= ( b >> 47 );

			seed = static_cast< std::size_t >( b * kMul );
		}

		size_t doHash( VboBindings const & vbos
			, IboBinding const & ibo )
		{
			size_t result{ 0u };

			for ( auto & binding : vbos )
			{
				auto & vbo = binding.second;
				doHashCombine( result, vbo.bo );
				doHashCombine( result, vbo.offset );
			}

			if ( bool( ibo ) )
			{
				doHashCombine( result, ibo.value().bo );
				doHashCombine( result, ibo.value().offset );
			}

			return result;
		}

		size_t doHash( VkVertexInputAttributeDescription const & desc )
		{
			size_t result = 0u;
			doHashCombine( result, desc.binding );
			doHashCombine( result, desc.format );
			doHashCombine( result, desc.location );
			doHashCombine( result, desc.offset );
			return result;
		}

		size_t doHash( VkVertexInputBindingDescription const & desc )
		{
			size_t result = 0u;
			doHashCombine( result, desc.binding );
			doHashCombine( result, desc.inputRate );
			doHashCombine( result, desc.stride );
			return result;
		}

		size_t doHash( VkPipelineVertexInputStateCreateInfo const & state )
		{
			size_t result = 0u;
			auto const endAttribs = state.pVertexAttributeDescriptions + state.vertexAttributeDescriptionCount;

			for ( auto it = state.pVertexAttributeDescriptions; it != endAttribs; ++it )
			{
				doHashCombine( result, doHash( *it ) );
			}

			auto const endBindings = state.pVertexBindingDescriptions + state.vertexBindingDescriptionCount;

			for ( auto it = state.pVertexBindingDescriptions; it != endBindings; ++it )
			{
				doHashCombine( result, doHash( *it ) );
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
		, m_inputAssemblyState{ makeOptional( createInfo.pInputAssemblyState ) }
		, m_tessellationState{ makeOptional( createInfo.pTessellationState ) }
		, m_viewportState{ makeOptional( createInfo.pViewportState
			, m_viewports
			, m_scissors ) }
		, m_rasterizationState{ makeOptional( createInfo.pRasterizationState ) }
		, m_multisampleState{ makeOptional( createInfo.pMultisampleState ) }
		, m_depthStencilState{ makeOptional( createInfo.pDepthStencilState ) }
		, m_colorBlendState{ makeOptional( createInfo.pColorBlendState
			, m_colorBlendAttachments ) }
		, m_dynamicState{ makeOptional( createInfo.pDynamicState
			, m_dynamicStates ) }
		, m_layout{ createInfo.layout }
		, m_renderPass{ createInfo.renderPass }
		, m_subpass{ createInfo.subpass }
		, m_basePipelineHandle{ createInfo.basePipelineHandle }
		, m_basePipelineIndex{ createInfo.basePipelineIndex }
		, m_vertexInputStateHash{ ( createInfo.pVertexInputState
			? doHash( m_vertexInputState.value() )
			: 0u ) }
		, m_program{ m_device, m_stages }
	{
		auto context = get( device )->getContext();
		apply( m_device
			, context
			, m_colorBlendState.value() );
		apply( m_device
			, context
			, m_rasterizationState.value()
			, hasDynamicStateEnable( VK_DYNAMIC_STATE_LINE_WIDTH )
			, hasDynamicStateEnable( VK_DYNAMIC_STATE_DEPTH_BIAS ) );
		apply( m_device
			, context
			, m_depthStencilState.value() );
		apply( m_device
			, context
			, m_multisampleState.value() );
		apply( m_device
			, context
			, m_tessellationState.value() );
		ShaderDesc shaderDesc = m_program.link( context );
		m_constantsPcb.stageFlags = shaderDesc.stageFlags;
		uint32_t offset = 0u;

		for ( auto & constant : shaderDesc.constantsLayout )
		{
			m_constantsPcb.constants.push_back( { constant.format
				, constant.location
				, offset
				, constant.size
				, constant.arraySize } );
			offset += constant.size;
		}

		m_constantsPcb.size = offset;

		if ( get( get( m_device )->getInstance() )->isValidationEnabled() )
		{
			validatePipeline( context
				, m_layout
				, m_program.getProgram()
				, m_vertexInputState.value()
				, m_renderPass );
		}
	}

	Pipeline::Pipeline( VkDevice device
		, VkComputePipelineCreateInfo createInfo )
		: m_device{ device }
		, m_stages{ makeVector( &createInfo.stage, 1u ) }
		, m_layout{ createInfo.layout }
		, m_basePipelineHandle{ createInfo.basePipelineHandle }
		, m_basePipelineIndex{ createInfo.basePipelineIndex }
		, m_program{ m_device, m_stages }
	{
		auto context = get( device )->getContext();
		ShaderDesc shaderDesc = m_program.link( context );
		m_constantsPcb.stageFlags = shaderDesc.stageFlags;
		uint32_t offset = 0u;

		for ( auto & constant : shaderDesc.constantsLayout )
		{
			m_constantsPcb.constants.push_back( { constant.format
				, constant.location
				, offset
				, constant.size
				, constant.arraySize } );
			offset += constant.size;
		}

		m_constantsPcb.size = offset;

		if ( get( get( m_device )->getInstance() )->isValidationEnabled() )
		{
			validatePipeline( context
				, m_layout
				, m_program.getProgram()
				, m_vertexInputState.value()
				, m_renderPass );
		}
	}

	Pipeline::~Pipeline()
	{
	}

	PushConstantsDesc Pipeline::findPushConstantBuffer( PushConstantsDesc const & pushConstants )const
	{
		PushConstantsDesc result{ m_constantsPcb };
		result.offset = pushConstants.offset;
		result.size = pushConstants.size;
		result.data = pushConstants.data;
		return result;
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
				, type ) );

		for ( auto & binding : vbos )
		{
			auto & vbo = binding.second;
			m_connections.emplace( vbo.bo, get( vbo.buffer )->onDestroy.connect( [this]( GLuint name )
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
							result = vbo.vbo == name;
						}
					}

					if ( !result && bool( pair.second->hasIbo() ) )
					{
						result = pair.second->getIbo().ibo == name;
					}

					return result;
				} );

				if ( it != m_geometryBuffers.end() )
				{
					m_geometryBuffers.erase( it, m_geometryBuffers.end() );
				}
			} ) );
		}

		return *m_geometryBuffers.back().second;
	}
}
