#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkComponentSwizzle convert( ashes::ComponentSwizzle const & value )
	{
		switch ( value )
		{
		case ashes::ComponentSwizzle::eIdentity:
			return VK_COMPONENT_SWIZZLE_IDENTITY;

		case ashes::ComponentSwizzle::eZero:
			return VK_COMPONENT_SWIZZLE_ZERO;

		case ashes::ComponentSwizzle::eOne:
			return VK_COMPONENT_SWIZZLE_ONE;

		case ashes::ComponentSwizzle::eR:
			return VK_COMPONENT_SWIZZLE_R;

		case ashes::ComponentSwizzle::eG:
			return VK_COMPONENT_SWIZZLE_G;

		case ashes::ComponentSwizzle::eB:
			return VK_COMPONENT_SWIZZLE_B;

		case ashes::ComponentSwizzle::eA:
			return VK_COMPONENT_SWIZZLE_A;

		default:
			assert( false && "Unsupported ComponentSwizzle" );
			return VK_COMPONENT_SWIZZLE_IDENTITY;
		}
	}
}
