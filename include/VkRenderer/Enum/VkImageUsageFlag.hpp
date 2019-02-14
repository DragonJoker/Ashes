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
	*	Convertit un ashes::ImageUsageFlags en VkImageUsageFlags.
	*\param[in] flags
	*	Le ashes::ImageUsageFlags.
	*\return
	*	Le VkImageUsageFlags.
	*/
	VkImageUsageFlags convert( ashes::ImageUsageFlags const & flags );
	/**
	*\brief
	*	Convertit un VkImageUsageFlags en ashes::ImageUsageFlags.
	*\param[in] flags
	*	Le VkImageUsageFlags.
	*\return
	*	Le ashes::ImageUsageFlags.
	*/
	ashes::ImageUsageFlags convertImageUsageFlags( VkImageUsageFlags const & flags );
}
