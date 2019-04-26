/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "D3D11RendererPrerequisites.hpp"

namespace ashes::d3d11
{
	RECT makeScissor( VkRect2D const & scissor )
	{
		return RECT
		{
			LONG( scissor.offset.x ),
			LONG( scissor.offset.y ),
			LONG( scissor.offset.x + scissor.extent.width ),
			LONG( scissor.offset.y + scissor.extent.height )
		};
	}

	RECT makeScissor( VkExtent2D const & extent )
	{
		return RECT
		{
			0L,
			0L,
			LONG( extent.width ),
			LONG( extent.height )
		};
	}
}
