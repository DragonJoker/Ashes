/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/TestDownloadMemoryCommand.hpp"

namespace ashes::test
{
	DownloadMemoryCommand::DownloadMemoryCommand( VkDevice device
		, ObjectMemory const *
		, VkDeviceSize offset
		, VkDeviceSize size )
		: CommandBase{ device }
		, m_range{ offset, size }
	{
	}

	void DownloadMemoryCommand::apply()const
	{
	}

	CommandPtr DownloadMemoryCommand::clone()const
	{
		return std::make_unique< DownloadMemoryCommand >( *this );
	}
}
