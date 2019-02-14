/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include <Ashes/Image/SubresourceLayout.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::SubresourceLayout en VkImageSubresourceRange.
	*\param[in] flags
	*	Le ashes::SubresourceLayout.
	*\return
	*	Le VkSubresourceLayout.
	*/
	VkSubresourceLayout convert( ashes::SubresourceLayout const & range );
	/**
	*\brief
	*	Convertit un VkSubresourceLayout en ashes::SubresourceLayout.
	*\param[in] flags
	*	Le VkSubresourceLayout.
	*\return
	*	Le ashes::SubresourceLayout.
	*/
	ashes::SubresourceLayout convert( VkSubresourceLayout const & range );
}
