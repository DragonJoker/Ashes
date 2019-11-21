/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/GlRendererCommon/GlCommonPrerequisites.hpp"

namespace ashes::gl
{
	uint32_t getScreenCount();
	void getScreenDesc( uint32_t index
		, std::string & name
		, VkDisplayPropertiesKHR & displayProps
		, std::vector< VkDisplayModeParametersKHR > & displayModesParams );
}
