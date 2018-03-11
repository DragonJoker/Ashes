/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkRect2D convert( renderer::Scissor const & scissor )
	{
		return VkRect2D
		{
			scissor.offset.x,
			scissor.offset.y,
			scissor.size.width,
			scissor.size.height
		};
	}
}
