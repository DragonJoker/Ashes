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
	*	Convertit un ashes::ColourComponentFlag en VkColorComponentFlags.
	*\param[in] flags
	*	Le ashes::ColourComponentFlag.
	*\return
	*	Le VkColorComponentFlags.
	*/
	VkColorComponentFlags convert( ashes::ColourComponentFlags const & flags );
}
