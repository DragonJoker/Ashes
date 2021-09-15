/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/TestSetDepthBiasCommand.hpp"

namespace ashes::test
{
	SetDepthBiasCommand::SetDepthBiasCommand( VkDevice device
		, float
		, float
		, float )
		: CommandBase{ device }
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
