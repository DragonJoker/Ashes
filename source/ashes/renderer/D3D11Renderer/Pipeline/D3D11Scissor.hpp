/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

namespace ashes::D3D11_NAMESPACE
{
	RECT makeScissor( VkRect2D const & scissor );
	RECT makeScissor( VkExtent2D const & extent );

	template< typename IterT >
	std::vector< RECT > makeScissors( IterT begin
		, IterT end )
	{
		std::vector< RECT > result;

		for ( auto it = begin; it != end; ++it )
		{
			result.push_back( makeScissor( *it ) );
		}

		return result;
	}

	template< typename IterT >
	std::vector< RECT > makeScissors( IterT begin
		, size_t count )
	{
		return makeScissors( begin, begin + count );
	}

	inline std::vector< RECT > makeScissors( VkRect2D const * begin
		, size_t count )
	{
		if ( !begin )
		{
			return std::vector< RECT >{};
		}

		return makeScissors( begin, begin + count );
	}

	inline std::vector< RECT > makeScissors( VkRect2D * begin
		, size_t count )
	{
		if ( !begin )
		{
			return std::vector< RECT >{};
		}

		return makeScissors( begin, begin + count );
	}

	inline std::vector< RECT > makeScissors( VkExtent2D const * begin
		, size_t count )
	{
		if ( !begin )
		{
			return std::vector< RECT >{};
		}

		return makeScissors( begin, begin + count );
	}

	inline std::vector< RECT > makeScissors( VkExtent2D * begin
		, size_t count )
	{
		if ( !begin )
		{
			return std::vector< RECT >{};
		}

		return makeScissors( begin, begin + count );
	}
}
