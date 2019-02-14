#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkCompositeAlphaFlagsKHR convert( ashes::CompositeAlphaFlags const & flags )
	{
		VkCompositeAlphaFlagsKHR result{ 0u };

		if ( ashes::checkFlag( flags, ashes::CompositeAlphaFlag::eOpaque ) )
		{
			result |= VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		}

		if ( ashes::checkFlag( flags, ashes::CompositeAlphaFlag::ePreMultiplied ) )
		{
			result |= VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR;
		}

		if ( ashes::checkFlag( flags, ashes::CompositeAlphaFlag::ePostMultiplied ) )
		{
			result |= VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR;
		}

		if ( ashes::checkFlag( flags, ashes::CompositeAlphaFlag::eInherit ) )
		{
			result |= VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR;
		}

		return result;
	}

	ashes::CompositeAlphaFlags convertCompositeAlpha( VkCompositeAlphaFlagsKHR const & flags )
	{
		ashes::CompositeAlphaFlags result{ 0u };

		if ( ashes::checkFlag( flags, VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR ) )
		{
			result |= ashes::CompositeAlphaFlag::eOpaque;
		}

		if ( ashes::checkFlag( flags, VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR ) )
		{
			result |= ashes::CompositeAlphaFlag::ePreMultiplied;
		}

		if ( ashes::checkFlag( flags, VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR ) )
		{
			result |= ashes::CompositeAlphaFlag::ePostMultiplied;
		}

		if ( ashes::checkFlag( flags, VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR ) )
		{
			result |= ashes::CompositeAlphaFlag::eInherit;
		}

		return result;
	}
}
