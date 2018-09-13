/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include <AshesPrerequisites.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::QueueFlags en VkQueueFlags.
	*\param[in] flags
	*	Le ashes::QueueFlags.
	*\return
	*	Le VkQueueFlags.
	*/
	VkQueueFlags convert( ashes::QueueFlags const & flags );
	/**
	*\brief
	*	Convertit un VkQueueFlags en ashes::QueueFlags.
	*\param[in] flags
	*	Le VkQueueFlags.
	*\return
	*	Le ashes::QueueFlags.
	*/
	ashes::QueueFlags convertQueueFlags( VkQueueFlags const & flags );
}
