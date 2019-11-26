/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/TestScissorCommand.hpp"

#include "Core/TestDevice.hpp"

namespace ashes::test
{
	ScissorCommand::ScissorCommand( VkDevice device
		, uint32_t first
		, VkScissorArray const & scissors )
		: CommandBase{ device }
	{
	}

	void ScissorCommand::apply()const
	{
	}

	CommandPtr ScissorCommand::clone()const
	{
		return std::make_unique< ScissorCommand >( *this );
	}
}
