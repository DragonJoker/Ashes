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
	*	Convertit un ashes::SubpassDescriptionFlags en VkSubpassDescriptionFlags.
	*\param[in] flags
	*	Le ashes::SubpassDescriptionFlags.
	*\return
	*	Le VkSubpassDescriptionFlags.
	*/
	VkSubpassDescriptionFlags convert( ashes::SubpassDescriptionFlags const & flags );
	/**
	*\brief
	*	Convertit un VkSubpassDescriptionFlags en ashes::SubpassDescriptionFlags.
	*\param[in] flags
	*	Le VkSubpassDescriptionFlags.
	*\return
	*	Le ashes::SubpassDescriptionFlags.
	*/
	ashes::SubpassDescriptionFlags convertSubpassDescriptionFlags( VkSubpassDescriptionFlags const & flags );
}
