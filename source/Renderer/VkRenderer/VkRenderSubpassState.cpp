/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#include "VkRenderSubpassState.hpp"

namespace vk_renderer
{
	vk::RenderSubpassState convert( renderer::RenderSubpassState const & state )
	{
		return vk::RenderSubpassState
		{
			convert( state.getPipelineStage() ),
			convert( state.getAccess() )
		};
	}
}
