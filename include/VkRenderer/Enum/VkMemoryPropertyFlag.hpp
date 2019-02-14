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
	*	Convertit un ashes::MemoryPropertyFlags en VkMemoryPropertyFlags.
	*\param[in] flags
	*	Le ashes::MemoryPropertyFlags.
	*\return
	*	Le VkMemoryPropertyFlags.
	*/
	VkMemoryPropertyFlags convert( ashes::MemoryPropertyFlags const & flags );
}
