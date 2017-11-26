/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#include "ColourBlendStateAttachment.hpp"

namespace renderer
{
	ColourBlendStateAttachment::ColourBlendStateAttachment( bool blendEnable
		, BlendFactor srcColorBlendFactor
		, BlendFactor dstColorBlendFactor
		, BlendOp colorBlendOp
		, BlendFactor srcAlphaBlendFactor
		, BlendFactor dstAlphaBlendFactor
		, BlendOp alphaBlendOp
		, ColourComponentFlags colorWriteMask )
		: m_state
		{
			blendEnable,
			convert( srcColorBlendFactor ),
			convert( dstColorBlendFactor ),
			convert( colorBlendOp ),
			convert( srcAlphaBlendFactor ),
			convert( dstAlphaBlendFactor ),
			convert( alphaBlendOp ),
			convert( colorWriteMask ),
		}
	{
	}
}
