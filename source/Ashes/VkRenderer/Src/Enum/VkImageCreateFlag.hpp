/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include <AshesPrerequisites.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::ImageCreateFlags en VkImageCreateFlags.
	*\param[in] flags
	*	Le ashes::ImageCreateFlags.
	*\return
	*	Le VkImageCreateFlags.
	*/
	VkImageCreateFlags convert( ashes::ImageCreateFlags const & flags );
	/**
	*\brief
	*	Convertit un VkAccessFlags en ashes::AccessFlags.
	*\param[in] flags
	*	Le VkAccessFlags.
	*\return
	*	Le ashes::AccessFlags.
	*/
	ashes::ImageCreateFlags convertImageCreateFlags( VkImageCreateFlags const & flags );
}
