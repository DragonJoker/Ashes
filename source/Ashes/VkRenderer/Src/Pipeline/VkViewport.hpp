/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include <Pipeline/Viewport.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::Viewport en VkViewport.
	*\param[in] flags
	*	Le ashes::Viewport.
	*\return
	*	Le VkViewport.
	*/
	VkViewport convert( ashes::Viewport const & viewport );
}
