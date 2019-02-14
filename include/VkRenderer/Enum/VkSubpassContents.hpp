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
	*	Convertit un ashes::SubpassContents en VkSubpassContents.
	*\param[in] value
	*	Le ashes::SubpassContents.
	*\return
	*	Le VkSubpassContents.
	*/
	VkSubpassContents convert( ashes::SubpassContents const & value );
}
