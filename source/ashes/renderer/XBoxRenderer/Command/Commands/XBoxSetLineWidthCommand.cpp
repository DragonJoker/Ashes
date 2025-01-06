/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/XBoxSetLineWidthCommand.hpp"

namespace ashes::xbox
{
	SetLineWidthCommand::SetLineWidthCommand( VkDevice device
		, float width )
		: CommandBase{ device }
		, m_width{ width }
	{
	}

	void SetLineWidthCommand::apply( Context const & context )const
	{
	}

	CommandPtr SetLineWidthCommand::clone()const
	{
		return std::make_unique< SetLineWidthCommand >( *this );
	}
}
