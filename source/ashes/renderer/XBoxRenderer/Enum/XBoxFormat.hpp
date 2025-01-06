/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

namespace ashes::xbox
{
	DXGI_FORMAT getDxgiFormat( VkFormat const & format )noexcept;
	DXGI_FORMAT getDxgiFormatGroup( DXGI_FORMAT fmt )noexcept;
	DXGI_FORMAT getTextureFormat( VkFormat const & format )noexcept;
	DXGI_FORMAT getSRVFormat( VkFormat const & format )noexcept;
	DXGI_FORMAT getRTVFormat( VkFormat const & format )noexcept;
	DXGI_FORMAT getUAVFormat( VkFormat const & format )noexcept;
	DXGI_FORMAT getBufferFormat( VkFormat const & format )noexcept;
	VkFormat getVkFormat( DXGI_FORMAT const & format )noexcept;
}
