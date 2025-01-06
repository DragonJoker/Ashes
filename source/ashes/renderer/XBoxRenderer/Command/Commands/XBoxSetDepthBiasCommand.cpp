/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/XBoxSetDepthBiasCommand.hpp"

namespace ashes::xbox
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

	void SetDepthBiasCommand::apply( Context const & context )const
	{
	}

	CommandPtr SetDepthBiasCommand::clone()const
	{
		return std::make_unique< SetDepthBiasCommand >( *this );
	}
}
