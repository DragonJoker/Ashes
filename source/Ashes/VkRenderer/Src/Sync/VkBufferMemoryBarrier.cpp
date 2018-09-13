/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#include "VkRendererPrerequisites.hpp"

#include "Buffer/VkBuffer.hpp"

namespace vk_renderer
{
	VkBufferMemoryBarrier convert( ashes::BufferMemoryBarrier const & barrier )
	{
		return VkBufferMemoryBarrier
		{
			VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER,
			nullptr,
			convert( barrier.getSrcAccessMask() ),
			convert( barrier.getDstAccessMask() ),
			barrier.getSrcQueueFamilyIndex(),
			barrier.getDstQueueFamilyIndex(),
			static_cast< Buffer const & >( barrier.getBuffer() ),
			barrier.getOffset(),
			barrier.getSize()
		};
	}

	ashes::BufferMemoryBarrier convert( Buffer const & bufferBase, VkBufferMemoryBarrier const & barrier )
	{
		return ashes::BufferMemoryBarrier
		{
			convertAccessFlags( barrier.srcAccessMask ),
			convertAccessFlags( barrier.dstAccessMask ),
			barrier.srcQueueFamilyIndex,
			barrier.dstQueueFamilyIndex,
			bufferBase,
			barrier.offset,
			barrier.size
		};
	}
}
