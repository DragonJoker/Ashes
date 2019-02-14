/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkPipelineViewportStateCreateInfo convert( ashes::ViewportState const & state
		, std::vector< VkViewport > & viewports
		, std::vector< VkRect2D > & scissors )
	{
		scissors = convert< VkRect2D >( state.scissors );
		viewports = convert< VkViewport >( state.viewports );
		return VkPipelineViewportStateCreateInfo
		{
			VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
			nullptr,
			0,                                                                    // flags
			std::max( 1u, uint32_t( viewports.size() ) ),                         // viewportCount
			viewports.empty() ? nullptr : viewports.data(),                       // pViewports
			std::max( 1u, uint32_t( scissors.size() ) ),                          // scissorCount
			scissors.empty() ? nullptr : scissors.data(),                         // pScissors
		};
	}
}
