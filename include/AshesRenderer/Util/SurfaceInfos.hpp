/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.h>

#include <string>
#include <vector>

namespace ashes
{
	void getSurfaceInfos( std::string & type
		, std::vector< VkSurfaceFormatKHR > & formats
		, VkSurfaceCapabilitiesKHR & capabilities );
}

