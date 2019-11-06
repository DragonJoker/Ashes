/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/TestCopyImageCommand.hpp"

namespace ashes::test
{
	CopyImageCommand::CopyImageCommand( VkDevice device
		, VkImageCopy const & copyInfo
		, VkImage src
		, VkImage dst )
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
