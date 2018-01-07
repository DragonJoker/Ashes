/*
This file belongs to VkLib.
See LICENSE file in root folder.
*/
#pragma once

#include "vulkan/vulkan.h"

namespace vk_renderer
{

#define VK_LIB_FUNCTION( fun ) extern PFN_vk##fun fun;
#include "VulkanFunctionsList.inl"

}
