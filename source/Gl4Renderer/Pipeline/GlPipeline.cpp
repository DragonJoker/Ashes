#include "Pipeline/GlPipeline.hpp"

#include "Buffer/GlBuffer.hpp"
#include "Command/Commands/GlBindPipelineCommand.hpp"
#include "Core/GlDevice.hpp"
#include "Core/GlInstance.hpp"
#include "Miscellaneous/GlValidator.hpp"
#include "Pipeline/GlPipelineLayout.hpp"
#include "RenderPass/GlRenderPass.hpp"
#include "Shader/GlShaderModule.hpp"

#include <algorithm>

#if defined( interface )
#	undef interface
#endif

namespace gl_renderer
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

	Pipeline::Pipeline( Device const & device
		, PipelineLayout const & layout
		, RenderPass const & renderPass
		, VkGraphicsPipelineCreateInfo createInfo )
		: m_device{ device }
		, m_layout{ layout }
		, m_renderPass{ renderPass }
		, m_createInfo{ createInfo }
		, m_ssState{ m_createInfo.pStages, m_createInfo.pStages + m_createInfo.stageCount ) }
		, m_vertexInputState{ *m_createInfo.pVertexInputState }
		, m_iaState{ *m_createInfo.pInputAssemblyState }
		, m_cbState{ *m_createInfo.pColorBlendState }
		, m_rsState{ *m_createInfo.pRasterizationState }
		, m_dsState{ VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO, nullptr, 0u, false, true, VK_COMPARE_OP_LESS,  }
		, m_msState{ *m_createInfo.pMultisampleState }
		, m_viewportState{ *m_createInfo.pViewportState }
		, m_vertexInputStateHash{ doHash( m_vertexInputState ) }
		, m_program{ m_device, m_ssState }
	{
		if ( m_createInfo.pDepthStencilState )
		{
			m_dsState = *m_createInfo.pDepthStencilState;
		}

		if ( m_createInfo.pTessellationState )
		{
			m_tsState = *m_createInfo.pTessellationState;
		}

		auto context = device.getContext();
		apply( m_device
			, context
			, m_cbState );
		apply( m_device
			, context
			, m_rsState
			, hasDynamicStateEnable( VK_DYNAMIC_STATE_LINE_WIDTH )
			, hasDynamicStateEnable( VK_DYNAMIC_STATE_DEPTH_BIAS ) );
		apply( m_device
			, context
			, m_dsState );
		apply( m_device
			, context
			, m_msState );
		apply( m_device
			, context
			, m_tsState );
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

		if ( static_cast< Instance const & >( m_device.getInstance() ).isValidationEnabled() )
		{
			validatePipeline( context
				, m_layout
				, m_program.getProgram()
				, m_vertexInputState
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
		, ashes::IndexType type )const
	{
		size_t hash = doHash( vbos, ibo );
		m_geometryBuffers.emplace_back( hash, std::make_unique< GeometryBuffers >( m_device, vbos, ibo, m_vertexInputState, type ) );

		for ( auto & binding : vbos )
		{
			auto & vbo = binding.second;
			m_connections.emplace( vbo.bo, vbo.buffer->onDestroy.connect( [this]( GLuint name )
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
