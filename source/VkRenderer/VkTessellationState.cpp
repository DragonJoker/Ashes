/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#include "VkTessellationState.hpp"

namespace vk_renderer
{
	vk::TessellationState convert( renderer::TessellationState const & state )
	{
		return vk::TessellationState
		{
			convert( state.getFlags() ),
			state.getControlPoints()
		};
	}
}
