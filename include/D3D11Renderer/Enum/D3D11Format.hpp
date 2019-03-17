/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include <Ashes/AshesPrerequisites.hpp>

#include <Ashes/Miscellaneous/Extent2D.hpp>

namespace d3d11_renderer
{
	DXGI_FORMAT getDxgiFormat( ashes::Format const & format )noexcept;
	DXGI_FORMAT getTextureFormat( ashes::Format const & format )noexcept;
	DXGI_FORMAT getSRVFormat( ashes::Format const & format )noexcept;
	DXGI_FORMAT getRTVFormat( ashes::Format const & format )noexcept;
	DXGI_FORMAT getUAVFormat( ashes::Format const & format )noexcept;
	DXGI_FORMAT getBufferFormat( ashes::Format const & format )noexcept;
	ashes::Format convert( DXGI_FORMAT const & format )noexcept;

	struct BlockSize
	{
		ashes::Extent2D block;
		uint32_t size;
	};

	BlockSize getBlockSize( ashes::Format format );
}
