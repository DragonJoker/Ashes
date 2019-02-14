/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___VkRenderer_BufferCopy_HPP___
#define ___VkRenderer_BufferCopy_HPP___
#pragma once

#include <Ashes/Miscellaneous/BufferCopy.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::BufferCopy en VkBufferCopy.
	*\param[in] mode
	*	Le ashes::BufferCopy.
	*\return
	*	Le VkBufferCopy.
	*/
	VkBufferCopy convert( ashes::BufferCopy const & value );
}

#endif
