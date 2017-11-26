/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#include "ColourBlendState.hpp"
#include "ColourBlendStateAttachment.hpp"

namespace renderer
{
	namespace
	{
		ColourBlendState doCreateDefault()
		{
			ColourBlendState result;
			result.addAttachment( ColourBlendStateAttachment{} );
			return result;
		}
	}

	ColourBlendState::ColourBlendState( bool logicOpEnable
		, LogicOp logicOp
		, utils::Vec4 const & blendConstants )
		: m_state{ logicOpEnable
			, convert( logicOp )
			, { blendConstants.r, blendConstants.g, blendConstants.b, blendConstants.a } }
	{
	}

	ColourBlendState const & ColourBlendState::createDefault()
	{
		static ColourBlendState const default = doCreateDefault();
		return default;
	}

	void ColourBlendState::addAttachment( ColourBlendStateAttachment const & attachment )
	{
		m_state.addAttachment( attachment.getAttachment() );
	}
}
