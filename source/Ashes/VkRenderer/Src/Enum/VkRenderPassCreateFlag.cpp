#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkRenderPassCreateFlags convert( ashes::RenderPassCreateFlags const & flags )
	{
		return VkRenderPassCreateFlags{ 0 };
	}

	ashes::RenderPassCreateFlags convertRenderPassCreateFlags( VkRenderPassCreateFlags const & flags )
	{
		return ashes::RenderPassCreateFlags{ 0 };
	}
}
