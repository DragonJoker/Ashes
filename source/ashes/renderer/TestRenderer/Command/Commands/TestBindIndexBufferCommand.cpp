/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/TestBindIndexBufferCommand.hpp"

#include "Buffer/TestBuffer.hpp"

#include "ashestest_api.hpp"

namespace ashes::test
{
	BindIndexBufferCommand::BindIndexBufferCommand( VkDevice device
		, VkBuffer
		, uint64_t
		, VkIndexType )
		: CommandBase{ device }
	{
	}

	void BindIndexBufferCommand::apply()const
	{
	}

	CommandPtr BindIndexBufferCommand::clone()const
	{
		return std::make_unique< BindIndexBufferCommand >( *this );
	}
}
