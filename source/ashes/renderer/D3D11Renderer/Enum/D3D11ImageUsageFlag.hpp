/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

namespace ashes::D3D11_NAMESPACE
{
	bool isRenderTarget( VkImageUsageFlags const & flags )noexcept;
	bool isStorage( VkImageUsageFlags const & flags )noexcept;
	bool isSampled( VkImageUsageFlags const & flags )noexcept;
	bool isRenderable( VkImageUsageFlags const & flags
		, VkFormat format
		, uint32_t mipLevels )noexcept;
	bool isMipmapped( VkImageUsageFlags const & flags
		, VkFormat format
		, uint32_t mipLevels )noexcept;
}
