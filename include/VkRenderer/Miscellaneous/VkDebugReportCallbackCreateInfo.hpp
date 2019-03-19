/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___VkRenderer_DebugReportCallbackCreateInfo_HPP___
#define ___VkRenderer_DebugReportCallbackCreateInfo_HPP___
#pragma once

#include <Ashes/Miscellaneous/DebugReportCallbackCreateInfo.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::DebugReportCallbackCreateInfo en VkDebugReportCallbackCreateInfoEXT.
	*\param[in] mode
	*	Le ashes::DebugReportCallbackCreateInfo.
	*\return
	*	Le VkDebugReportCallbackCreateInfoEXT.
	*/
	VkDebugReportCallbackCreateInfoEXT convert( ashes::DebugReportCallbackCreateInfo const & value );
}

#endif
