/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#include "D3D11RendererPrerequisites.hpp"

namespace d3d11_renderer
{
	RECT makeScissor( renderer::Scissor const & scissor )
	{
		return RECT
		{
			LONG( scissor.offset.x ),
			LONG( scissor.offset.y ),
			LONG( scissor.offset.x + scissor.size.width ),
			LONG( scissor.offset.y + scissor.size.height )
		};
	}

	RECT makeScissor( renderer::Extent2D const & extent )
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
