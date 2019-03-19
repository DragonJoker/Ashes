/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include <Ashes/AshesPrerequisites.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::Result en VkResult.
	*\param[in] value
	*	Le ashes::Result.
	*\return
	*	Le VkResult.
	*/
	VkResult convert( ashes::Result const & value );
	/**
	*\brief
	*	Convertit un VkResult en ashes::Result.
	*\param[in] value
	*	Le VkResult.
	*\return
	*	Le ashes::Result.
	*/
	ashes::Result convert( VkResult const & value );
}
