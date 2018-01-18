/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#include "VkRenderSubpassState.hpp"

namespace vk_renderer
{
	RenderSubpassState convert( renderer::RenderSubpassState const & state )
	{
		return RenderSubpassState
		{
			state.getPipelineStage(),
			state.getAccess()
		};
	}
}
