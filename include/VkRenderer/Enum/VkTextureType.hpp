/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___VkRenderer_TextureType_HPP___
#define ___VkRenderer_TextureType_HPP___
#pragma once

#include <Ashes/AshesPrerequisites.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::ImageType en VkImageViewType.
	*\param[in] value
	*	Le ashes::ImageType.
	*\return
	*	Le VkImageViewType.
	*/
	VkImageType convert( ashes::ImageType const & value )noexcept;
}

#endif
