/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#include "VkViewport.hpp"

namespace vk_renderer
{
	VkViewport convert( renderer::Viewport const & viewport )
	{
		return VkViewport
		{
			float( viewport.getOffset().x ),
			float( viewport.getOffset().y ),
			float( viewport.getSize().x ),
			float( viewport.getSize().y ),
			0.0f,                 // minDepth
			1.0f                  // maxDepth
		};
	}
}
