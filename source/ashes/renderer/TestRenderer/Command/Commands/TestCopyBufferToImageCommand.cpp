/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/TestCopyBufferToImageCommand.hpp"

namespace ashes::test
{
	CopyBufferToImageCommand::CopyBufferToImageCommand( VkDevice device
		, VkBufferImageCopyArray const & copyInfos
		, VkBuffer src
		, VkImage dst )
		: CommandBase{ device }
	{
	}

	void CopyBufferToImageCommand::apply()const
	{
	}

	CommandPtr CopyBufferToImageCommand::clone()const
	{
		return std::make_unique< CopyBufferToImageCommand >( *this );
	}
}
