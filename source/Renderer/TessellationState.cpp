/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#include "TessellationState.hpp"

namespace renderer
{
	TessellationState::TessellationState( TessellationStateFlags flags
		, uint32_t patchControlPoints )
		: m_state
		{
			convert( flags ),
			patchControlPoints
		}
	{
	}
}
