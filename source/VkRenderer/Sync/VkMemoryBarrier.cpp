/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkMemoryBarrier convert( ashes::MemoryBarrier const & barrier )
	{
		return VkMemoryBarrier
		{
			VK_STRUCTURE_TYPE_MEMORY_BARRIER,
			nullptr,
			convert( barrier.srcAccessMask ),
			convert( barrier.dstAccessMask ),
		};
	}

	ashes::MemoryBarrier convert( VkMemoryBarrier const & barrier )
	{
		return ashes::MemoryBarrier
		{
			convertAccessFlags( barrier.srcAccessMask ),
			convertAccessFlags( barrier.dstAccessMask ),
		};
	}
}
