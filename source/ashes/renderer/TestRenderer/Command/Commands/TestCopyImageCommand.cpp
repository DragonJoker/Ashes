/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/TestCopyImageCommand.hpp"

namespace ashes::test
{
	CopyImageCommand::CopyImageCommand( VkDevice device
		, VkImageCopy const &
		, VkImage
		, VkImage )
		: CommandBase{ device }
	{
	}

	void CopyImageCommand::apply()const
	{
	}

	CommandPtr CopyImageCommand::clone()const
	{
		return std::make_unique< CopyImageCommand >( *this );
	}
}
