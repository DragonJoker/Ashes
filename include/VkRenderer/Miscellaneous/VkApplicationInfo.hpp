/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include <Ashes/Miscellaneous/ApplicationInfo.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::ApplicationInfo en VkApplicationInfo.
	*\param[in] mode
	*	Le ashes::ApplicationInfo.
	*\return
	*	Le VkApplicationInfo.
	*/
	VkApplicationInfo convert( ashes::ApplicationInfo const & value );
}
