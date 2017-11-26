/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#include "DepthStencilState.hpp"

#include "StencilOpState.hpp"

namespace renderer
{
	DepthStencilState::DepthStencilState( DepthStencilStateFlags flags
		, bool depthTestEnable
		, bool depthWriteEnable
		, CompareOp depthCompareOp
		, bool depthBoundsTestEnable
		, bool stencilTestEnable
		, StencilOpState const & front
		, StencilOpState const & back
		, float minDepthBounds
		, float maxDepthBounds )
		: m_state
		{
			flags,
			depthTestEnable,
			depthWriteEnable,
			convert( depthCompareOp ),
			depthBoundsTestEnable,
			stencilTestEnable,
			front.getState(),
			back.getState(),
			minDepthBounds,
			maxDepthBounds
		}
	{
	}
}
