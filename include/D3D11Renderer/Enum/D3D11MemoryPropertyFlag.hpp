/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

namespace ashes::d3d11
{
	bool isHostVisible( VkMemoryPropertyFlags const & flags );
	// Buffer
	D3D11_USAGE getUsage( VkMemoryPropertyFlags const & flags
		, VkBufferUsageFlags const & targets );
	D3D11_USAGE getUsage( VkMemoryPropertyFlags const & flags
		, VkImageUsageFlags const & usage );
	// Image
	UINT getCpuAccessFlags( VkMemoryPropertyFlags const & flags
		, VkBufferUsageFlags const & targets );
	UINT getCpuAccessFlags( VkMemoryPropertyFlags const & flags
		, VkImageUsageFlags const & usage );
}
