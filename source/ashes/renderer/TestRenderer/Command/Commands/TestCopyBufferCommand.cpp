/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/TestCopyBufferCommand.hpp"

namespace ashes::test
{
	CopyBufferCommand::CopyBufferCommand( VkDevice device
		, VkBufferCopy const & copyInfo
		, VkBuffer src
		, VkBuffer dst )
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
