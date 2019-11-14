/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/TestDrawIndexedIndirectCommand.hpp"

namespace ashes::test
{
	DrawIndexedIndirectCommand::DrawIndexedIndirectCommand( VkDevice device
		, VkBuffer buffer
		, VkDeviceSize offset
		, uint32_t drawCount
		, uint32_t stride
		, VkPrimitiveTopology mode
		, VkIndexType type
		, VbosBindingArray const & vbos )
		: CommandBase{ device }
		, m_buffer{ buffer }
		, m_offset{ uint32_t( offset ) }
		, m_drawCount{ drawCount }
		, m_stride{ stride }
		, m_vbos{ vbos }
	{
	}

	void DrawIndexedIndirectCommand::apply()const
	{
	}

	CommandPtr DrawIndexedIndirectCommand::clone()const
	{
		return std::make_unique< DrawIndexedIndirectCommand >( *this );
	}
}
