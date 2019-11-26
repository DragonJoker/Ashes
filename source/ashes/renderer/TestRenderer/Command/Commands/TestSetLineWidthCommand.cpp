/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/TestSetLineWidthCommand.hpp"

namespace ashes::test
{
	SetLineWidthCommand::SetLineWidthCommand( VkDevice device
		, float width )
		: CommandBase{ device }
		, m_width{ width }
	{
	}

	void SetLineWidthCommand::apply()const
	{
	}

	CommandPtr SetLineWidthCommand::clone()const
	{
		return std::make_unique< SetLineWidthCommand >( *this );
	}
}
