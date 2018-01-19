/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#include "Pipeline/VkScissor.hpp"

namespace vk_renderer
{
	VkRect2D convert( renderer::Scissor const & scissor )
	{
		return VkRect2D
		{
			scissor.getOffset().x,
			scissor.getOffset().y,
			uint32_t( scissor.getSize().x ),
			uint32_t( scissor.getSize().y )
		};
	}
}
