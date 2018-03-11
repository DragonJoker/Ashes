/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkViewport convert( renderer::Viewport const & viewport )
	{
		return VkViewport
		{
			float( viewport.offset.x ),
			float( viewport.offset.y ),
			float( viewport.size.width ),
			float( viewport.size.height ),
			viewport.minDepth,
			viewport.maxDepth
		};
	}
}
