/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include <Ashes/Image/ImageSubresourceRange.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::ImageSubresourceRange en VkImageSubresourceRange.
	*\param[in] flags
	*	Le ashes::ImageSubresourceRange.
	*\return
	*	Le VkImageSubresourceRange.
	*/
	VkImageSubresourceRange convert( ashes::ImageSubresourceRange const & range );
	/**
	*\brief
	*	Convertit un VkImageSubresourceRange en ashes::ImageSubresourceRange.
	*\param[in] flags
	*	Le VkImageSubresourceRange.
	*\return
	*	Le ashes::ImageSubresourceRange.
	*/
	ashes::ImageSubresourceRange convert( VkImageSubresourceRange const & range );
}
