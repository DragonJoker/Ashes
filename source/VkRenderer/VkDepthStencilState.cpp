/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#include "VkDepthStencilState.hpp"

#include "VkStencilOpState.hpp"

namespace vk_renderer
{
	vk::DepthStencilState convert( renderer::DepthStencilState const & state )
	{
		return vk::DepthStencilState
		{
			state.getFlags(),
			state.isDepthTestEnabled(),
			state.isDepthWriteEnabled(),
			convert( state.getDepthCompareOp() ),
			state.isDepthBoundsTestEnabled(),
			state.isStencilTestEnabled(),
			convert( state.getFrontStencilOp() ),
			convert( state.getBackStencilOp() ),
			state.getMinDepthBounds(),
			state.getMaxDepthBounds()
		};
	}
}
