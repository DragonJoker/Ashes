/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkSurfaceCapabilitiesKHR convert( ashes::SurfaceCapabilities const & value )
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

	ashes::SurfaceCapabilities convert( VkSurfaceCapabilitiesKHR const & value )
	{
		return ashes::SurfaceCapabilities
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
