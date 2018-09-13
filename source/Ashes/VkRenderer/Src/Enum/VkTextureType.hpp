/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___VkRenderer_TextureType_HPP___
#define ___VkRenderer_TextureType_HPP___
#pragma once

#include <AshesPrerequisites.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::TextureType en VkImageViewType.
	*\param[in] value
	*	Le ashes::TextureType.
	*\return
	*	Le VkImageViewType.
	*/
	VkImageType convert( ashes::TextureType const & value )noexcept;
}

#endif
