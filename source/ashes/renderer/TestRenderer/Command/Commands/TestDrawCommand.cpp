/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/TestDrawCommand.hpp"

namespace ashes::test
{
	DrawCommand::DrawCommand( VkDevice device
		, uint32_t vtxCount
		, uint32_t instCount
		, uint32_t firstVertex
		, uint32_t firstInstance
		, VkPrimitiveTopology mode
		, VbosBindingArray const & vbos )
		: CommandBase{ device }
		, m_vtxCount{ vtxCount }
		, m_instCount{ instCount }
		, m_firstVertex{ firstVertex }
		, m_firstInstance{ firstInstance }
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
