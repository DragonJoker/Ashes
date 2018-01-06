/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#include "VkScissor.hpp"

namespace vk_renderer
{
	vk::Scissor convert( renderer::Scissor const & scissor )
	{
		return vk::Scissor
		{
			scissor.getOffset().x,
			scissor.getOffset().y,
			uint32_t( scissor.getSize().x ),
			uint32_t( scissor.getSize().y )
		};
	}
}
