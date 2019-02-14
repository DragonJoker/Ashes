/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___VkRenderer_BufferImageCopy_HPP___
#define ___VkRenderer_BufferImageCopy_HPP___
#pragma once

#include <Ashes/Miscellaneous/BufferImageCopy.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::BufferImageCopy en VkBufferImageCopy.
	*\param[in] mode
	*	Le ashes::BufferImageCopy.
	*\return
	*	Le VkBufferImageCopy.
	*/
	VkBufferImageCopy convert( ashes::BufferImageCopy const & value );
}

#endif
