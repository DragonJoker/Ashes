/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___VkRenderer_TextureViewType_HPP___
#define ___VkRenderer_TextureViewType_HPP___
#pragma once

#include <Ashes/AshesPrerequisites.hpp>

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
	VkImageViewType convert( ashes::TextureViewType const & value )noexcept;
}

#endif
