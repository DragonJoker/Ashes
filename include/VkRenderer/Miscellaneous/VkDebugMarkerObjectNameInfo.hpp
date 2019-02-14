/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___VkRenderer_DebugMarkerObjectNameInfo_HPP___
#define ___VkRenderer_DebugMarkerObjectNameInfo_HPP___
#pragma once

#include <Ashes/Miscellaneous/DebugMarkerObjectNameInfo.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::DebugMarkerObjectNameInfo en VkDebugMarkerObjectNameInfoEXT.
	*\param[in] mode
	*	Le ashes::DebugMarkerObjectNameInfo.
	*\return
	*	Le VkDebugMarkerObjectNameInfoEXT.
	*/
	VkDebugMarkerObjectNameInfoEXT convert( ashes::DebugMarkerObjectNameInfo const & value );
}

#endif
