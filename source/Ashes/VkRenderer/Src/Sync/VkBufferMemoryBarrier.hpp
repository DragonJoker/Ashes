/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include <Sync/BufferMemoryBarrier.hpp>

namespace vk_renderer
{
	class Buffer;
	/**
	*\brief
	*	Convertit un ashes::BufferMemoryBarrier en VkBufferMemoryBarrier.
	*\param[in] flags
	*	Le ashes::BufferMemoryBarrier.
	*\return
	*	Le VkBufferMemoryBarrier.
	*/
	VkBufferMemoryBarrier convert( ashes::BufferMemoryBarrier const & barrier );
	/**
	*\brief
	*	Convertit un VkBufferMemoryBarrier en ashes::BufferMemoryBarrier.
	*\param[in] flags
	*	Le VkBufferMemoryBarrier.
	*\return
	*	Le ashes::BufferMemoryBarrier.
	*/
	ashes::BufferMemoryBarrier convert( Buffer const & bufferBase, VkBufferMemoryBarrier const & barrier );
}
