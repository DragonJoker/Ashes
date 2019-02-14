/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include <Ashes/AshesPrerequisites.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::MemoryHeapFlags en VkMemoryHeapFlags.
	*\param[in] flags
	*	Le ashes::MemoryHeapFlags.
	*\return
	*	Le VkMemoryHeapFlags.
	*/
	VkMemoryHeapFlags convert( ashes::MemoryHeapFlags const & flags );
	/**
	*\brief
	*	Convertit un VkMemoryHeapFlags en ashes::MemoryHeapFlags.
	*\param[in] flags
	*	Le VkMemoryHeapFlags.
	*\return
	*	Le ashes::MemoryHeapFlags.
	*/
	ashes::MemoryHeapFlags convertMemoryHeapFlags( VkMemoryHeapFlags const & flags );
}
