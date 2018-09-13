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
	*	Convertit un ashes::AccessFlags en VkAccessFlags.
	*\param[in] flags
	*	Le ashes::AccessFlags.
	*\return
	*	Le VkAccessFlags.
	*/
	VkAccessFlags convert( ashes::AccessFlags const & flags );
	/**
	*\brief
	*	Convertit un VkAccessFlags en ashes::AccessFlags.
	*\param[in] flags
	*	Le VkAccessFlags.
	*\return
	*	Le ashes::AccessFlags.
	*/
	ashes::AccessFlags convertAccessFlags( VkAccessFlags const & flags );
}
