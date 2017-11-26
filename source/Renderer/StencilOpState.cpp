/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#include "StencilOpState.hpp"

namespace renderer
{
	StencilOpState::StencilOpState( StencilOp failOp
		, StencilOp passOp
		, StencilOp depthFailOp
		, CompareOp compareOp
		, uint32_t compareMask
		, uint32_t writeMask
		, uint32_t reference )
		: m_state
		{
			convert( failOp ),
			convert( passOp ),
			convert( depthFailOp ),
			convert( compareOp ),
			compareMask,
			writeMask,
			reference,
		}
	{
	}
}
