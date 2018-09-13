/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___VkRenderer_ImageSubresourceLayers_HPP___
#define ___VkRenderer_ImageSubresourceLayers_HPP___
#pragma once

#include <Image/ImageSubresourceLayers.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::ImageSubresourceLayers en VkImageSubresourceLayers.
	*\param[in] value
	*	Le ashes::ImageSubresourceLayers.
	*\return
	*	Le VkImageSubresourceLayers.
	*/
	VkImageSubresourceLayers convert( ashes::ImageSubresourceLayers const & value );
}

#endif
