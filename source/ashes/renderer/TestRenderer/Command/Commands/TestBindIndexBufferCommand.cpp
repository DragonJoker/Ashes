/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/TestBindIndexBufferCommand.hpp"

#include "Buffer/TestBuffer.hpp"

#include "ashestest_api.hpp"

namespace ashes::test
{
	BindIndexBufferCommand::BindIndexBufferCommand( VkDevice device
		, VkBuffer ibo
		, uint64_t offset
		, VkIndexType indexType )
		: CommandBase{ device }
		, m_ibo{ ibo }
		, m_offset{ uint32_t( offset ) }
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
