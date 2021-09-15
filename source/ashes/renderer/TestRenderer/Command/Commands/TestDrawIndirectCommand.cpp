/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/TestDrawIndirectCommand.hpp"

#include "Buffer/TestBuffer.hpp"

#include "ashestest_api.hpp"

namespace ashes::test
{
	DrawIndirectCommand::DrawIndirectCommand( VkDevice device
		, VkBuffer
		, VkDeviceSize
		, uint32_t
		, uint32_t
		, VkPrimitiveTopology
		, VbosBindingArray const & vbos )
		: CommandBase{ device }
		, m_vbos{ vbos }
	{
	}

	void DrawIndirectCommand::apply()const
	{
	}

	CommandPtr DrawIndirectCommand::clone()const
	{
		return std::make_unique< DrawIndirectCommand >( *this );
	}
}
