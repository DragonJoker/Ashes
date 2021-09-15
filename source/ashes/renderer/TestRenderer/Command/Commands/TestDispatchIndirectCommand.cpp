/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/TestDispatchIndirectCommand.hpp"

namespace ashes::test
{
	DispatchIndirectCommand::DispatchIndirectCommand( VkDevice device
		, VkBuffer
		, VkDeviceSize )
		: CommandBase{ device }
	{
	}

	void DispatchIndirectCommand::apply()const
	{
	}

	CommandPtr DispatchIndirectCommand::clone()const
	{
		return std::make_unique< DispatchIndirectCommand >( *this );
	}
}
