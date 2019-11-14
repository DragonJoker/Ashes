/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

namespace ashes::d3d11
{
	bool isRenderTarget( VkImageUsageFlags const & flags );
	bool isStorage( VkImageUsageFlags const & flags );
	bool isSampled( VkImageUsageFlags const & flags );
	bool isRenderable( VkImageUsageFlags const & flags
		, VkFormat format
		, uint32_t mipLevels );
	bool isMipmapped( VkImageUsageFlags const & flags
		, VkFormat format
		, uint32_t mipLevels );
}
