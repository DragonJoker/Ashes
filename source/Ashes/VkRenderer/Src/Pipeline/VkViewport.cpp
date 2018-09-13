/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkViewport convert( ashes::Viewport const & viewport )
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
