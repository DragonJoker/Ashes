/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkViewport convert( renderer::Viewport const & viewport )
	{
		return VkViewport
		{
			float( viewport.getOffset()[0] ),
			float( viewport.getOffset()[1] ),
			float( viewport.getSize()[0] ),
			float( viewport.getSize()[1] ),
			viewport.getDepthBounds()[0],
			viewport.getDepthBounds()[1]
		};
	}
}
