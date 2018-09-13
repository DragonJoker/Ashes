/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#include "D3D11RendererPrerequisites.hpp"

namespace d3d11_renderer
{
	D3D11_VIEWPORT makeViewport( renderer::Viewport const & viewport )
	{
		return D3D11_VIEWPORT
		{
			float( viewport.offset.x ),
			float( viewport.offset.y ),
			float( viewport.size.width ),
			float( viewport.size.height ),
			viewport.minDepth,
			viewport.maxDepth
		};
	}
}
