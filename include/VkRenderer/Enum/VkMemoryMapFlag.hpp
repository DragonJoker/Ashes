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
	*	Convertit un ashes::MemoryMapFlags en VkMemoryMapFlags.
	*\param[in] format
	*	Le ashes::MemoryMapFlags.
	*\return
	*	Le VkMemoryMapFlags.
	*/
	VkMemoryMapFlags convert( ashes::MemoryMapFlags const & flags );
}
