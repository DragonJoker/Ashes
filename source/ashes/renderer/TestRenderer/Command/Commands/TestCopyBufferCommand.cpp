/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/TestCopyBufferCommand.hpp"

namespace ashes::test
{
	CopyBufferCommand::CopyBufferCommand( VkDevice device
		, VkBufferCopy const &
		, VkBuffer
		, VkBuffer )
		: CommandBase{ device }
	{
	}

	void CopyBufferCommand::apply()const
	{
	}

	CommandPtr CopyBufferCommand::clone()const
	{
		return std::make_unique< CopyBufferCommand >( *this );
	}
}
