/*
This file belongs to VkLib.
See LICENSE file in root folder.
*/
#include "RenderSubpassState.hpp"

namespace renderer
{
	RenderSubpassState::RenderSubpassState( PipelineStageFlags pipelineStage
		, AccessFlags access )
		: m_state{ convert( pipelineStage )
		, convert( access ) }
	{
	}
}
