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
	*	Convertit un ashes::QueryControlFlags en VkQueryControlFlags.
	*\param[in] flags
	*	Le ashes::QueryControlFlags.
	*\return
	*	Le VkQueryControlFlags.
	*/
	VkQueryControlFlags convert( ashes::QueryControlFlags const & flags );
}
