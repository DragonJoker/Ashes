#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkCompositeAlphaFlagsKHR convert( renderer::CompositeAlphaFlags const & flags )
	{
		VkCompositeAlphaFlagsKHR result{ 0u };

		if ( renderer::checkFlag( flags, renderer::CompositeAlphaFlag::eOpaque ) )
		{
			result |= VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		}

		if ( renderer::checkFlag( flags, renderer::CompositeAlphaFlag::ePreMultiplied ) )
		{
			result |= VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR;
		}

		if ( renderer::checkFlag( flags, renderer::CompositeAlphaFlag::ePostMultiplied ) )
		{
			result |= VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR;
		}

		if ( renderer::checkFlag( flags, renderer::CompositeAlphaFlag::eInherit ) )
		{
			result |= VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR;
		}

		return result;
	}

	renderer::CompositeAlphaFlags convertCompositeAlpha( VkCompositeAlphaFlagsKHR const & flags )
	{
		renderer::CompositeAlphaFlags result{ 0u };

		if ( renderer::checkFlag( flags, VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR ) )
		{
			result |= renderer::CompositeAlphaFlag::eOpaque;
		}

		if ( renderer::checkFlag( flags, VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR ) )
		{
			result |= renderer::CompositeAlphaFlag::ePreMultiplied;
		}

		if ( renderer::checkFlag( flags, VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR ) )
		{
			result |= renderer::CompositeAlphaFlag::ePostMultiplied;
		}

		if ( renderer::checkFlag( flags, VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR ) )
		{
			result |= renderer::CompositeAlphaFlag::eInherit;
		}

		return result;
	}
}
