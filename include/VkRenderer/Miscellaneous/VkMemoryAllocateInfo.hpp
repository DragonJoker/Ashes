/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include <Ashes/Miscellaneous/MemoryAllocateInfo.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::MemoryAllocateInfo en VkMemoryAllocateInfo.
	*\param[in] mode
	*	Le ashes::MemoryAllocateInfo.
	*\return
	*	Le VkMemoryAllocateInfo.
	*/
	VkMemoryAllocateInfo convert( ashes::MemoryAllocateInfo const & value );
}

