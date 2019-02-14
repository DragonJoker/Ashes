/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include <Ashes/AshesPrerequisites.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::Filter en VkFilter.
	*\param[in] filter
	*	Le ashes::Filter.
	*\return
	*	Le VkFilter.
	*/
	VkFilter convert( ashes::Filter const & filter );
}
