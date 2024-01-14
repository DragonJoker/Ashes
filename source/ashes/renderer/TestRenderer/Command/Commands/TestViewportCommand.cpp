/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/TestViewportCommand.hpp"

namespace ashes::test
{
	ViewportCommand::ViewportCommand( VkDevice device
		, uint32_t
		, VkViewportArray const & )
		: CommandBase{ device }
	{
	}

	void ViewportCommand::apply()const
	{
	}

	CommandPtr ViewportCommand::clone()const
	{
		return std::make_unique< ViewportCommand >( *this );
	}
}
