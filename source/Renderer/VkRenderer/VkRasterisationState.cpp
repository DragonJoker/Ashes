/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#include "VkRasterisationState.hpp"

namespace vk_renderer
{
	vk::RasterisationState convert( renderer::RasterisationState const & state )
	{
		return vk::RasterisationState
		{
			state.getFlags(),
			state.isDepthClampEnabled(),
			state.isRasteriserDiscardEnabled(),
			convert( state.getPolygonMode() ),
			convert( state.getCullMode() ),
			convert( state.getFrontFace() ),
			state.isDepthBiasEnabled(),
			state.getDepthBiasConstantFactor(),
			state.getDepthBiasClamp(),
			state.getDepthBiasSlopeFactor(),
			state.getLineWidth()
		};
	}
}
