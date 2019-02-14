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
	*	Convertit un ashes::QueryResultFlags en VkQueryResultFlags.
	*\param[in] flags
	*	Le ashes::QueryResultFlags.
	*\return
	*	Le VkQueryResultFlags.
	*/
	VkQueryResultFlags convert( ashes::QueryResultFlags const & flags );
}
