/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___VkRenderer_ImageBlit_HPP___
#define ___VkRenderer_ImageBlit_HPP___
#pragma once

#include <Ashes/Miscellaneous/ImageBlit.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::ImageBlit en VkImageBlit.
	*\param[in] mode
	*	Le ashes::ImageBlit.
	*\return
	*	Le VkImageBlit.
	*/
	VkImageBlit convert( ashes::ImageBlit const & value );
}

#endif
