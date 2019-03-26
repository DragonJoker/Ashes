/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_SwapChainCreateFlag_HPP___
#define ___Ashes_SwapChainCreateFlag_HPP___
#pragma once

namespace ashes
{
	/**
	*\brief
	*	Enumération des indicateurs de création de swapchain.
	*/
	enum class SwapChainCreateFlag
		: int32_t
	{
		/**
		*\~english
		*	No flag.
		*/
		eNone,
		/**
		*\~english
		*	Specifies that images created from the swapchain (i.e. with the swapchain member of ImageSwapchainCreateInfo set to this swapchain’s handle)
		*	must use VK_IMAGE_CREATE_SPLIT_INSTANCE_BIND_REGIONS_BIT.
		*/
		eSplitInstanceBindRegions = 0x00000001,
		/**
		*\~english
		*	Specifies that images created from the swapchain are protected images.
		*/
		eProtected = 0x00000002,
		/**
		*\~english
		*	Specifies that the images of the swapchain can be used to create a ImageView with a different format
		*	than what the swapchain was created with.
		*	The list of allowed image view formats are specified by chaining an instance of the ImageFormatListCreateInfo
		*	structure to the pNext chain of SwapChainCreateInfo.
		*	In addition, this flag also specifies that the swapchain can be created with usage flags that are not supported
		*	for the format the swapchain is created with but are supported for at least one of the allowed image view formats.
		*/
		eMutableFormat = 0x00000004,
	};
	Ashes_ImplementFlag( SwapChainCreateFlag )
}

#endif
