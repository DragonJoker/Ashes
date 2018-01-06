/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#include "VkColourBlendState.hpp"

namespace vk_renderer
{
	vk::ColourBlendState convert( renderer::ColourBlendState const & state )
	{
		vk::ColourBlendState result
		{
			state.isLogicOpEnabled(),
			convert( state.getLogicOp() ),
			{
				state.getBlendConstants()[0],
				state.getBlendConstants()[1],
				state.getBlendConstants()[2],
				state.getBlendConstants()[3],
			}
		};

		for ( auto & attach : state )
		{
			result.addAttachment( convert( attach ) );
		}

		return result;
	}
}
