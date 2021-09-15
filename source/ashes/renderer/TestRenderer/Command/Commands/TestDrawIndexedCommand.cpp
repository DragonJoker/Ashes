/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/TestDrawIndexedCommand.hpp"

namespace ashes::test
{
	DrawIndexedCommand::DrawIndexedCommand( VkDevice device
		, uint32_t
		, uint32_t
		, uint32_t
		, uint32_t
		, uint32_t
		, VkPrimitiveTopology
		, VkIndexType
		, VbosBindingArray const & vbos )
		: CommandBase{ device }
		, m_vbos{ vbos }
	{
	}

	void DrawIndexedCommand::apply()const
	{
	}

	CommandPtr DrawIndexedCommand::clone()const
	{
		return std::make_unique< DrawIndexedCommand >( *this );
	}
}
