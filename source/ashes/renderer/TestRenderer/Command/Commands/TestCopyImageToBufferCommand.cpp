/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/TestCopyImageToBufferCommand.hpp"

namespace ashes::test
{
	CopyImageToBufferCommand::CopyImageToBufferCommand( VkDevice device
		, VkBufferImageCopyArray const & copyInfo
		, VkImage src
		, VkBuffer dst )
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
