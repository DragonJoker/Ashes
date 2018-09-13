/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include <Image/ImageSubresource.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::ImageSubresource en VkImageSubresource.
	*\param[in] flags
	*	Le ashes::ImageSubresource.
	*\return
	*	Le VkImageSubresource.
	*/
	VkImageSubresource convert( ashes::ImageSubresource const & range );
	/**
	*\brief
	*	Convertit un VkImageSubresource en ashes::ImageSubresource.
	*\param[in] flags
	*	Le VkImageSubresource.
	*\return
	*	Le ashes::ImageSubresource.
	*/
	ashes::ImageSubresource convert( VkImageSubresource const & range );
}
