#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkColorSpaceKHR convert( renderer::ColorSpace const & value )
	{
		switch ( value )
		{
		case renderer::ColorSpace::eSRGBNonLinear:
			return VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;

		default:
			assert( false && "Unsupported ColorSpace" );
			return VkColorSpaceKHR( ~( 0u ) );
		}
	}

	renderer::ColorSpace convertColorSpace( VkColorSpaceKHR const & value )
	{
		switch ( value )
		{
		case VK_COLOR_SPACE_SRGB_NONLINEAR_KHR:
			return renderer::ColorSpace::eSRGBNonLinear;

		default:
			assert( false && "Unsupported ColorSpace" );
			return renderer::ColorSpace( ~( 0u ) );
		}
	}
}
