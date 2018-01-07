/*
This file belongs to VkLib.
See LICENSE file in root folder.
*/
#include "VkLibPrerequisites.hpp"

namespace vk
{

#define VK_LIB_FUNCTION( fun ) PFN_vk##fun fun;
#include "FunctionsList.inl"

}
