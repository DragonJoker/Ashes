/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include <Ashes/Sync/ImageMemoryBarrier.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::ImageMemoryBarrier en VkImageMemoryBarrier.
	*\param[in] barrier
	*	Le ashes::ImageMemoryBarrier.
	*\return
	*	Le VkImageMemoryBarrier.
	*/
	VkImageMemoryBarrier convert( ashes::ImageMemoryBarrier const & barrier );
}
