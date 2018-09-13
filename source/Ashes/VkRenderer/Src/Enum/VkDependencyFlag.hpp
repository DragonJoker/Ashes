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
	*	Convertit un ashes::DependencyFlags en VkDependencyFlags.
	*\param[in] flags
	*	Le ashes::DependencyFlags.
	*\return
	*	Le VkDependencyFlags.
	*/
	VkDependencyFlags convert( ashes::DependencyFlags const & flags );
	/**
	*\brief
	*	Convertit un VkAccessFlags en ashes::DependencyFlags.
	*\param[in] flags
	*	Le VkDependencyFlags.
	*\return
	*	Le ashes::DependencyFlags.
	*/
	ashes::DependencyFlags convertDependencyFlags( VkDependencyFlags const & flags );
}
