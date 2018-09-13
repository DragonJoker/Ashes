/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Pipeline/Viewport.hpp"

namespace ashes
{
	Viewport::Viewport( uint32_t width
		, uint32_t height
		, int32_t x
		, int32_t y
		, float minZ
		, float maxZ )
		: offset{ x, y }
		, size{ width, height }
		, minDepth{ minZ }
		, maxDepth{ maxZ }
	{
	}

	Viewport::Viewport( Offset2D offset
		, Extent2D size
		, float minZ
		, float maxZ )
		: offset{ std::move( offset ) }
		, size{ std::move( size ) }
		, minDepth{ minZ }
		, maxDepth{ maxZ }
	{
	}
}
