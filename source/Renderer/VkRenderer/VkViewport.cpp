/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#include "VkViewport.hpp"

namespace vk_renderer
{
	vk::Viewport convert( renderer::Viewport const & viewport )
	{
		return vk::Viewport
		{
			uint32_t( viewport.getSize().x ),
			uint32_t( viewport.getSize().y ),
			viewport.getOffset().x,
			viewport.getOffset().y
		};
	}
}
