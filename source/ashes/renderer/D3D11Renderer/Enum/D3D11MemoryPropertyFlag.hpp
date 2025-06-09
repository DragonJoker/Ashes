/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

namespace ashes::D3D11_NAMESPACE
{
	bool isHostVisible( VkMemoryPropertyFlags const & flags );
	// Buffer
	D3D11_USAGE getBufferUsage( VkMemoryPropertyFlags const & flags
		, VkBufferUsageFlags const & targets );
	UINT getCpuBufferAccessFlags( VkMemoryPropertyFlags const & flags
		, VkBufferUsageFlags const & targets );
	// Image
	D3D11_USAGE getImageUsage( VkMemoryPropertyFlags const & flags
		, VkImageUsageFlags const & usage );
	UINT getCpuImageAccessFlags( VkMemoryPropertyFlags const & flags
		, VkImageUsageFlags const & usage );
}
