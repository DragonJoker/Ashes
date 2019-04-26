/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

namespace ashes::d3d11
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
}
