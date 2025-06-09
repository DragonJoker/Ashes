/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

namespace ashes::D3D11_NAMESPACE
{
	D3D11_MAP getBufferMapFlags( VkMemoryPropertyFlags const & memory
		, VkBufferUsageFlags const & usage );
	D3D11_MAP getImageMapFlags( VkMemoryPropertyFlags const & memory
		, VkImageUsageFlags const & usage );
}
