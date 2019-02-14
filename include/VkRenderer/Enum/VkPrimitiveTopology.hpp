/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include <Ashes/AshesPrerequisites.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::PrimitiveTopology en VkPrimitiveTopology.
	*\param[in] format
	*	Le ashes::PrimitiveTopology.
	*\return
	*	Le VkPrimitiveTopology.
	*/
	VkPrimitiveTopology convert( ashes::PrimitiveTopology const & topology );
}
