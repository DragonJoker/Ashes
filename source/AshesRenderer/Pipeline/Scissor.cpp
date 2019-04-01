/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "AshesRenderer/Pipeline/Scissor.hpp"

namespace ashes
{
	Scissor::Scissor( int32_t x
		, int32_t y
		, uint32_t width
		, uint32_t height )
		: offset{ x, y }
		, size{ width, height }
	{
	}

	Scissor::Scissor( Offset2D offset
		, Extent2D size )
		: offset{ std::move( offset ) }
		, size{ std::move( size ) }
	{
	}
}
