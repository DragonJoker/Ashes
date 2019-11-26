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
		, VkBuffer buffer
		, VkDeviceSize offset
		, uint32_t drawCount
		, uint32_t stride
		, VkPrimitiveTopology mode
		, VbosBindingArray const & vbos )
		: CommandBase{ device }
		, m_buffer{ buffer }
		, m_offset{ uint32_t( offset ) }
		, m_drawCount{ drawCount }
		, m_stride{ stride }
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
