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
	*	Convertit un ashes::FenceCreateFlags en VkFenceCreateFlags.
	*\param[in] flags
	*	Le ashes::FenceCreateFlags.
	*\return
	*	Le VkFenceCreateFlags.
	*/
	VkFenceCreateFlags convert( ashes::FenceCreateFlags const & flags );
}
