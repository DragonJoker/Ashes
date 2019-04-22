/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "D3D11RendererPrerequisites.hpp"

namespace ashes::d3d11
{
	D3D11_DEPTH_STENCILOP_DESC convert( ashes::StencilOpState const & state )
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
