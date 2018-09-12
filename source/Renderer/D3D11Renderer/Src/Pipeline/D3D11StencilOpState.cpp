/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#include "D3D11RendererPrerequisites.hpp"

namespace d3d11_renderer
{
	D3D11_DEPTH_STENCILOP_DESC convert( renderer::StencilOpState const & state )
	{
		return D3D11_DEPTH_STENCILOP_DESC
		{
			convert( state.failOp ),
			convert( state.depthFailOp ),
			convert( state.passOp ),
			convert( state.compareOp ),
		};
	}
}
