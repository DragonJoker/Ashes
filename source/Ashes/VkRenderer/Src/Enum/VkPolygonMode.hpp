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
	*	Convertit un ashes::PolygonMode en VkPolygonMode.
	*\param[in] value
	*	Le ashes::PolygonMode.
	*\return
	*	Le VkPolygonMode.
	*/
	VkPolygonMode convert( ashes::PolygonMode const & value );
}
