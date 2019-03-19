/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___VkRenderer_SwapChainCreateInfo_HPP___
#define ___VkRenderer_SwapChainCreateInfo_HPP___
#pragma once

#include <Ashes/Miscellaneous/SwapChainCreateInfo.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::SwapChainCreateInfo en VkSwapchainCreateInfoKHR.
	*\param[in] mode
	*	Le ashes::SwapChainCreateInfo.
	*\return
	*	Le VkSwapchainCreateInfoKHR.
	*/
	VkSwapchainCreateInfoKHR convert( ashes::SwapChainCreateInfo const & value );
}

#endif
