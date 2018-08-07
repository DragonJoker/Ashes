/*
This file belongs to RendererLib.
See LICENSE file in root folder
*/
#include "Miscellaneous/VkSurfaceCapabilities.hpp"

#include "Enum/VkCompositeAlphaFlag.hpp"
#include "Enum/VkImageUsageFlag.hpp"
#include "Enum/VkSurfaceTransformFlag.hpp"
#include "Miscellaneous/VkExtent2D.hpp"

namespace vk_renderer
{
	VkSurfaceCapabilitiesKHR convert( renderer::SurfaceCapabilities const & value )
	{
		return VkSurfaceCapabilitiesKHR
		{
			value.minImageCount,
			value.maxImageCount,
			convert( value.currentExtent ),
			convert( value.minImageExtent ),
			convert( value.maxImageExtent ),
			value.maxImageArrayLayers,
			convert( value.supportedTransforms ),
			convert( value.currentTransform ),
			convert( value.supportedCompositeAlpha ),
			convert( value.supportedUsageFlags ),
		};
	}

	renderer::SurfaceCapabilities convert( VkSurfaceCapabilitiesKHR const & value )
	{
		return renderer::SurfaceCapabilities
		{
			value.minImageCount,
			value.maxImageCount,
			convert( value.currentExtent ),
			convert( value.minImageExtent ),
			convert( value.maxImageExtent ),
			value.maxImageArrayLayers,
			convertSurfaceTransform( value.supportedTransforms ),
			convert( value.currentTransform ),
			convertCompositeAlpha( value.supportedCompositeAlpha ),
			convertImageUsageFlags( value.supportedUsageFlags ),
		};
	}
}
