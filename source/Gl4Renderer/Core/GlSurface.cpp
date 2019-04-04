/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Core/GlSurface.hpp"

#include "Core/GlInstance.hpp"
#include "Core/GlPhysicalDevice.hpp"

#include "ashesgl4_api.hpp"

namespace ashes::gl4
{
	void SurfaceKHR::getSurfaceInfos( VkSurfaceFormatArrayKHR & formats
		, VkSurfaceCapabilitiesKHR & capabilities )
	{
		formats.push_back( { VK_FORMAT_R8G8B8A8_UNORM, VK_COLORSPACE_SRGB_NONLINEAR_KHR } );

		capabilities.minImageCount = 1u;
		capabilities.maxImageCount = 1u;
		capabilities.currentExtent.width = ~( 0u );
		capabilities.currentExtent.height = ~( 0u );
		capabilities.minImageExtent = capabilities.currentExtent;
		capabilities.maxImageExtent = capabilities.currentExtent;
		capabilities.maxImageArrayLayers = 1u;
		capabilities.supportedUsageFlags = 0u;
		capabilities.supportedTransforms = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
		capabilities.currentTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
		capabilities.supportedCompositeAlpha = VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR;
	}
}
