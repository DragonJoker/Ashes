#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkBorderColor convert( ashes::BorderColour const & value )noexcept
	{
		switch ( value )
		{
		case ashes::BorderColour::eFloatTransparentBlack:
			return VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;

		case ashes::BorderColour::eIntTransparentBlack:
			return VK_BORDER_COLOR_INT_TRANSPARENT_BLACK;

		case ashes::BorderColour::eFloatOpaqueBlack:
			return VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK;

		case ashes::BorderColour::eIntOpaqueBlack:
			return VK_BORDER_COLOR_INT_OPAQUE_BLACK;

		case ashes::BorderColour::eFloatOpaqueWhite:
			return VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;

		case ashes::BorderColour::eIntOpaqueWhite:
			return VK_BORDER_COLOR_INT_OPAQUE_WHITE;

		default:
			assert( false && "Unsupported border colour" );
			return VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
		}
	}
}
