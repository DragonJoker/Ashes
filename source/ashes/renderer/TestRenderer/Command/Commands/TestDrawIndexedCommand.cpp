/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/TestDrawIndexedCommand.hpp"

namespace ashes::test
{
	DrawIndexedCommand::DrawIndexedCommand( VkDevice device
		, uint32_t indexCount
		, uint32_t instCount
		, uint32_t firstIndex
		, uint32_t vertexOffset
		, uint32_t firstInstance
		, VkPrimitiveTopology mode
		, VkIndexType type
		, VbosBindingArray const & vbos )
		: CommandBase{ device }
		, m_indexCount{ indexCount }
		, m_instCount{ instCount }
		, m_firstIndex{ firstIndex }
		, m_vertexOffset{ vertexOffset }
		, m_firstInstance{ firstInstance }
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
