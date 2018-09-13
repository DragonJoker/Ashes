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
	*	Convertit un ashes::ImageLayout en VkImageLayout.
	*\param[in] layout
	*	Le ashes::ImageLayout.
	*\return
	*	Le VkImageLayout.
	*/
	VkImageLayout convert( ashes::ImageLayout const & layout );
	/**
	*\brief
	*	Convertit un VkImageLayout en ashes::ImageLayout.
	*\param[in] layout
	*	Le VkImageLayout.
	*\return
	*	Le ashes::ImageLayout.
	*/
	ashes::ImageLayout convertImageLayout( VkImageLayout const & layout );
}
