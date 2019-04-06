/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include <Ashes/AshesPrerequisites.hpp>

#include <Ashes/Miscellaneous/Extent2D.hpp>

namespace d3d11_renderer
{
	DXGI_FORMAT getDxgiFormat( VkFormat const & format )noexcept;
	DXGI_FORMAT getTextureFormat( VkFormat const & format )noexcept;
	DXGI_FORMAT getSRVFormat( VkFormat const & format )noexcept;
	DXGI_FORMAT getRTVFormat( VkFormat const & format )noexcept;
	DXGI_FORMAT getUAVFormat( VkFormat const & format )noexcept;
	DXGI_FORMAT getBufferFormat( VkFormat const & format )noexcept;
	VkFormat convert( DXGI_FORMAT const & format )noexcept;

	struct BlockSize
	{
		VkExtent2D block;
		uint32_t size;
	};

	BlockSize getBlockSize( VkFormat format );
}
