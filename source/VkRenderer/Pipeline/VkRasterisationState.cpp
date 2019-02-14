/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkPipelineRasterizationStateCreateInfo convert( ashes::RasterisationState const & state )
	{
		return VkPipelineRasterizationStateCreateInfo
		{
			VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
			nullptr,
			convert( state.flags ),
			state.depthClampEnable,
			state.rasteriserDiscardEnable,
			convert( state.polygonMode ),
			convert( state.cullMode ),
			convert( state.frontFace ),
			state.depthBiasEnable,
			state.depthBiasConstantFactor,
			state.depthBiasClamp,
			state.depthBiasSlopeFactor,
			state.lineWidth
		};
	}
}
