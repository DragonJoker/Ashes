/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/TestDrawCommand.hpp"

namespace ashes::test
{
	DrawCommand::DrawCommand( VkDevice device
		, uint32_t
		, uint32_t
		, uint32_t
		, uint32_t
		, VkPrimitiveTopology
		, VbosBindingArray const & vbos )
		: CommandBase{ device }
		, m_vbos{ vbos }
	{
	}

	void DrawCommand::apply()const
	{
	}

	CommandPtr DrawCommand::clone()const
	{
		return std::make_unique< DrawCommand >( *this );
	}
}
