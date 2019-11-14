/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/TestUploadMemoryCommand.hpp"

#include "Miscellaneous/TestDeviceMemory.hpp"

#include "ashestest_api.hpp"

namespace ashes::test
{
	UploadMemoryCommand::UploadMemoryCommand( VkDevice device
		, ObjectMemory const * memory
		, VkDeviceSize offset
		, VkDeviceSize size )
		: CommandBase{ device }
		, m_memory{ memory }
		, m_range{ offset, size }
	{
	}

	void UploadMemoryCommand::apply()const
	{
		get( m_memory->deviceMemory )->updateUpload( *m_memory
			, m_range.first
			, m_range.second );
	}

	CommandPtr UploadMemoryCommand::clone()const
	{
		return std::make_unique< UploadMemoryCommand >( *this );
	}
}
