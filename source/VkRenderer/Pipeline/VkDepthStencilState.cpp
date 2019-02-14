/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkPipelineDepthStencilStateCreateInfo convert( ashes::DepthStencilState const & state )
	{
		return VkPipelineDepthStencilStateCreateInfo
		{
			VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
			nullptr,
			convert( state.flags ),
			state.depthTestEnable,
			state.depthWriteEnable,
			convert( state.depthCompareOp ),
			state.depthBoundsTestEnable,
			state.stencilTestEnable,
			convert( state.front ),
			convert( state.back ),
			state.minDepthBounds,
			state.maxDepthBounds
		};
	}
}
