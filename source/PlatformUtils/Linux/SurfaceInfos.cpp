#include "PlatformUtils/SurfaceInfos.hpp"

#if ASHES_XLIB

namespace utils
{
	void getSurfaceInfos( ashes::WindowHandle const & handle
		, std::string & type
		, std::vector< ashes::SurfaceFormat > & formats
		, ashes::SurfaceCapabilities & capabilities )
	{
		type = "VK_KHR_xlib_surface";

		formats.push_back( { ashes::Format::eR8G8B8A8_UNORM, ashes::ColorSpace::eSRGBNonLinear } );

		capabilities.minImageCount = 1u;
		capabilities.maxImageCount = 1u;
		capabilities.currentExtent.width = ~( 0u );
		capabilities.currentExtent.height = ~( 0u );
		capabilities.minImageExtent = capabilities.currentExtent;
		capabilities.maxImageExtent = capabilities.currentExtent;
		capabilities.maxImageArrayLayers = 1u;
		capabilities.supportedTransforms = ashes::SurfaceTransformFlag::eIdentity;
		capabilities.currentTransform = ashes::SurfaceTransformFlag::eIdentity;
		capabilities.supportedCompositeAlpha = ashes::CompositeAlphaFlag::eInherit;
		capabilities.supportedUsageFlags = ashes::ImageUsageFlag::eUndefined;
	}
}

#endif
