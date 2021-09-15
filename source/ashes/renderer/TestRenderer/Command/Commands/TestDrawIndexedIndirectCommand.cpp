/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/TestDrawIndexedIndirectCommand.hpp"

namespace ashes::test
{
	DrawIndexedIndirectCommand::DrawIndexedIndirectCommand( VkDevice device
		, VkBuffer
		, VkDeviceSize
		, uint32_t
		, uint32_t
		, VkPrimitiveTopology
		, VkIndexType
		, VbosBindingArray const & vbos )
		: CommandBase{ device }
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
