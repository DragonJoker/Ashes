/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/TestCopyImageToBufferCommand.hpp"

namespace ashes::test
{
	CopyImageToBufferCommand::CopyImageToBufferCommand( VkDevice device
		, VkBufferImageCopyArray const &
		, VkImage
		, VkBuffer )
		: CommandBase{ device }
	{
	}

	void CopyImageToBufferCommand::apply()const
	{
	}

	CommandPtr CopyImageToBufferCommand::clone()const
	{
		return std::make_unique< CopyImageToBufferCommand >( *this );
	}
}
