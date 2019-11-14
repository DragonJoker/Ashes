/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

namespace ashes::d3d11
{
	D3D11_VIEWPORT makeViewport( VkViewport const & viewport );

	template< typename IterT >
	std::vector< D3D11_VIEWPORT > makeViewports( IterT begin
		, IterT end )
	{
		std::vector< D3D11_VIEWPORT > result;

		for ( auto it = begin; it != end; ++it )
		{
			result.push_back( makeViewport( *it ) );
		}

		return result;
	}

	template< typename IterT >
	std::vector< D3D11_VIEWPORT > makeViewports( IterT begin
		, size_t count )
	{
		return makeViewports( begin, begin + count );
	}

	inline std::vector< D3D11_VIEWPORT > makeViewports( VkViewport * begin
		, size_t count )
	{
		if ( !begin )
		{
			return std::vector< D3D11_VIEWPORT >{};
		}

		return makeViewports( begin, begin + count );
	}

	inline std::vector< D3D11_VIEWPORT > makeViewports( VkViewport const * begin
		, size_t count )
	{
		if ( !begin )
		{
			return std::vector< D3D11_VIEWPORT >{};
		}

		return makeViewports( begin, begin + count );
	}
}
