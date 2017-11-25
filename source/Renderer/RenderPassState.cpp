/*
This file belongs to VkLib.
See LICENSE file in root folder.
*/
#include "RenderPassState.hpp"

namespace renderer
{
	RenderPassState::RenderPassState( PipelineStageFlags pipelineStage
		, AccessFlags access
		, std::vector< ImageLayout > const & imageLayouts )
		: m_state{ convert( pipelineStage )
			, convert( access )
			, convert< VkImageLayout >( imageLayouts ) }
	{
	}
}
