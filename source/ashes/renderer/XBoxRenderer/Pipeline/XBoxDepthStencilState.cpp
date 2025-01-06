/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "XBoxRendererPrerequisites.hpp"

namespace ashes::xbox
{
	D3D11_DEPTH_STENCIL_DESC convert( VkPipelineDepthStencilStateCreateInfo const & state )
	{
		return D3D11_DEPTH_STENCIL_DESC
		{
			BOOL( state.depthTestEnable ),
			( state.depthWriteEnable
				? D3D11_DEPTH_WRITE_MASK_ALL
				: D3D11_DEPTH_WRITE_MASK_ZERO ),
			getComparisonFunc( state.depthCompareOp ),
			BOOL( state.stencilTestEnable ),
			UINT8( state.front.compareMask ),
			UINT8( state.front.writeMask ),
			convert( state.front ),
			convert( state.back ),
		};
	}
}
