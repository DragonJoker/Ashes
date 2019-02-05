/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include <Sync/MemoryBarrier.hpp>

#if defined( MemoryBarrier )
#	undef MemoryBarrier
#endif

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::MemoryBarrier en VkMemoryBarrier.
	*\param[in] flags
	*	Le ashes::MemoryBarrier.
	*\return
	*	Le VkMemoryBarrier.
	*/
	VkMemoryBarrier convert( ashes::MemoryBarrier const & barrier );
	/**
	*\brief
	*	Convertit un VkMemoryBarrier en ashes::MemoryBarrier.
	*\param[in] flags
	*	Le VkMemoryBarrier.
	*\return
	*	Le ashes::MemoryBarrier.
	*/
	ashes::MemoryBarrier convert( VkMemoryBarrier const & barrier );
}
