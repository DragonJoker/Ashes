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
	*	Convertit un ashes::ImageAspectFlags en VkImageAspectFlags.
	*\param[in] flags
	*	Le ashes::ImageAspectFlags.
	*\return
	*	Le VkImageAspectFlags.
	*/
	VkImageAspectFlags convert( ashes::ImageAspectFlags const & flags );
	/**
	*\brief
	*	Convertit un VkImageAspectFlags en ashes::ImageAspectFlags.
	*\param[in] flags
	*	Le VkImageAspectFlags.
	*\return
	*	Le ashes::ImageAspectFlags.
	*/
	ashes::ImageAspectFlags convertAspectMask( VkImageAspectFlags const & flags );
}
