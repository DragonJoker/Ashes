#include "Pipeline/TestPipeline.hpp"

#include "Core/TestDevice.hpp"
#include "Shader/TestShaderModule.hpp"

#include "ashestest_api.hpp"

namespace ashes::test
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

			for ( auto & desc : makeArrayView( state.pVertexAttributeDescriptions, state.vertexAttributeDescriptionCount ) )
			{
				doHashCombine( result, doHash( desc ) );
			}

			for ( auto & desc : makeArrayView( state.pVertexBindingDescriptions, state.vertexBindingDescriptionCount ) )
			{
				doHashCombine( result, doHash( desc ) );
			}

			return result;
		}
	}

	Pipeline::Pipeline( VkDevice device
		, VkGraphicsPipelineCreateInfo createInfo )
		: m_device{ device }
		, m_layout{ createInfo.layout }
		, m_vertexInputState{ ( createInfo.pVertexInputState
			? deepCopy( *createInfo.pVertexInputState, m_vertexBindingDescriptions, m_vertexAttributeDescriptions )
			: VkPipelineVertexInputStateCreateInfo{} ) }
		, m_inputAssemblyState{ ( createInfo.pInputAssemblyState
			? deepCopy( *createInfo.pInputAssemblyState )
			: VkPipelineInputAssemblyStateCreateInfo{} ) }
		, m_viewportState{ ( createInfo.pViewportState
			? deepCopy( *createInfo.pViewportState, m_stateViewports, m_stateScissors )
			: VkPipelineViewportStateCreateInfo{} ) }
		, m_rasterizationState{ ( createInfo.pRasterizationState
			? deepCopy( *createInfo.pRasterizationState )
			: VkPipelineRasterizationStateCreateInfo{} ) }
		, m_multisampleState{ ( createInfo.pMultisampleState
			? deepCopy( *createInfo.pMultisampleState )
			: VkPipelineMultisampleStateCreateInfo{} ) }
		, m_depthStencilState{ ( createInfo.pDepthStencilState
			? Optional< VkPipelineDepthStencilStateCreateInfo >( deepCopy( *createInfo.pDepthStencilState ) )
			: ashes::nullopt ) }
		, m_colorBlendState{ ( createInfo.pColorBlendState
			? deepCopy( *createInfo.pColorBlendState, m_colorBlendStateAttachments )
			: VkPipelineColorBlendStateCreateInfo{} ) }
		, m_dynamicState{ ( createInfo.pDynamicState
			? deepCopy( *createInfo.pDynamicState, m_dynamicStates )
			: VkPipelineDynamicStateCreateInfo{} ) }
		, m_vertexInputStateHash{ doHash( m_vertexInputState ) }
	{
	}

	Pipeline::Pipeline( VkDevice device
		, VkComputePipelineCreateInfo createInfo )
		: m_device{ device }
		, m_layout{ createInfo.layout }
	{
	}

	VkDescriptorSetLayoutArray const & Pipeline::getDescriptorsLayouts()const
	{
		return get( m_layout )->getDescriptorsLayouts();
	}
}
