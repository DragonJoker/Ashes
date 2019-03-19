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
	*	Convertit un ashes::SwapChainCreateFlags en VkSwapchainCreateFlagsKHR.
	*\param[in] flags
	*	Le ashes::SwapChainCreateFlags.
	*\return
	*	Le VkSwapchainCreateFlagsKHR.
	*/
	VkSwapchainCreateFlagsKHR convert( ashes::SwapChainCreateFlags const & flags );
}
