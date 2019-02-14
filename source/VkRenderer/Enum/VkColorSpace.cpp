#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkColorSpaceKHR convert( ashes::ColorSpace const & value )
	{
		switch ( value )
		{
		case ashes::ColorSpace::eSRGBNonLinear:
			return VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;

		default:
			assert( false && "Unsupported ColorSpace" );
			return VkColorSpaceKHR( ~( 0u ) );
		}
	}

	ashes::ColorSpace convertColorSpace( VkColorSpaceKHR const & value )
	{
		switch ( value )
		{
		case VK_COLOR_SPACE_SRGB_NONLINEAR_KHR:
			return ashes::ColorSpace::eSRGBNonLinear;

		default:
			assert( false && "Unsupported ColorSpace" );
			return ashes::ColorSpace( ~( 0u ) );
		}
	}
}
