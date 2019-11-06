/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/TestSetDepthBiasCommand.hpp"

namespace ashes::test
{
	SetDepthBiasCommand::SetDepthBiasCommand( VkDevice device
		, float constantFactor
		, float clamp
		, float slopeFactor )
		: CommandBase{ device }
		, m_constantFactor{ constantFactor }
		, m_clamp{ clamp }
		, m_slopeFactor{ slopeFactor }
	{
	}

	void SetDepthBiasCommand::apply()const
	{
	}

	CommandPtr SetDepthBiasCommand::clone()const
	{
		return std::make_unique< SetDepthBiasCommand >( *this );
	}
}
