/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "XBoxRendererPrerequisites.hpp"

namespace ashes::xbox
{
	D3D11_DEPTH_STENCILOP_DESC convert( VkStencilOpState const & state )
	{
		return D3D11_DEPTH_STENCILOP_DESC
		{
			getStencilOp( state.failOp ),
			getStencilOp( state.depthFailOp ),
			getStencilOp( state.passOp ),
			getComparisonFunc( state.compareOp ),
		};
	}
}
