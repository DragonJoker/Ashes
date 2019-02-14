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
	*	Convertit un ashes::BufferTargets en VkBufferUsageFlags.
	*\param[in] targets
	*	Le ashes::BufferTargets.
	*\return
	*	Le VkBufferUsageFlags.
	*/
	VkBufferUsageFlags convert( ashes::BufferTargets const & targets );
}
