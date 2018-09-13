/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___VkRenderer_ImageCopy_HPP___
#define ___VkRenderer_ImageCopy_HPP___
#pragma once

#include <Miscellaneous/ImageCopy.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::ImageCopy en VkImageCopy.
	*\param[in] mode
	*	Le ashes::ImageCopy.
	*\return
	*	Le VkImageCopy.
	*/
	VkImageCopy convert( ashes::ImageCopy const & value );
}

#endif
