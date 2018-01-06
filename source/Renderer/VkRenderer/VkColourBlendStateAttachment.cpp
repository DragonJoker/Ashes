/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#include "VkColourBlendStateAttachment.hpp"

namespace vk_renderer
{
	vk::ColourBlendStateAttachment convert( renderer::ColourBlendStateAttachment const & state )
	{
		return vk::ColourBlendStateAttachment
		{
			state.isBlendEnabled(),
			convert( state.getSrcColourBlendFactor() ),
			convert( state.getDstColourBlendFactor() ),
			convert( state.getColourBlendOp() ),
			convert( state.getSrcAlphaBlendFactor() ),
			convert( state.getDstAlphaBlendFactor() ),
			convert( state.getAlphaBlendOp() ),
			convert( state.getColourWriteMask() ),
		};
	}
}
