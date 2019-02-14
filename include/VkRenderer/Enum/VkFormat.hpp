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
	*	Convertit un ashes::Format en VkFormat.
	*\param[in] format
	*	Le ashes::Format.
	*\return
	*	Le VkFormat.
	*/
	VkFormat convert( ashes::Format const & format )noexcept;
	/**
	*\brief
	*	Convertit un VkFormat en ashes::Format.
	*\param[in] format
	*	Le VkFormat.
	*\return
	*	Le ashes::Format.
	*/
	ashes::Format convert( VkFormat const & format )noexcept;
}
