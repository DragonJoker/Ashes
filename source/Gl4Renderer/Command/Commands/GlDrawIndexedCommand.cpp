/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlDrawIndexedCommand.hpp"

#include "ashesgl4_api.hpp"

namespace ashes::gl4
{
	namespace
	{
		uint32_t getSize( VkIndexType type )
		{
			switch ( type )
			{
			case VK_INDEX_TYPE_UINT16:
				return 2u;
			case VK_INDEX_TYPE_UINT32:
				return 4u;
			default:
				assert( false && "Unsupported index type" );
				return 1u;
			}
		}
	}

	DrawIndexedCommand::DrawIndexedCommand( VkDevice device
		, uint32_t indexCount
		, uint32_t instCount
		, uint32_t firstIndex
		, uint32_t vertexOffset
		, uint32_t firstInstance
		, VkPrimitiveTopology mode
		, VkIndexType type )
		: CommandBase{ device }
		, m_indexCount{ indexCount }
		, m_instCount{ instCount }
		, m_firstIndex{ firstIndex }
		, m_vertexOffset{ vertexOffset }
		, m_firstInstance{ firstInstance }
		, m_mode{ convert( mode ) }
		, m_type{ convert( type ) }
		, m_size{ getSize( type ) }
	{
	}

	void DrawIndexedCommand::apply( ContextLock const & context )const
	{
		glLogCommand( "DrawIndexedCommand" );
		glLogCall( context
			, glDrawElementsInstancedBaseVertexBaseInstance
			, m_mode
			, m_indexCount
			, m_type
			, ( ( GLvoid * )( m_firstIndex * m_size ) )
			, m_instCount
			, m_vertexOffset
			, m_firstInstance );
	}

	CommandPtr DrawIndexedCommand::clone()const
	{
		return std::make_unique< DrawIndexedCommand >( *this );
	}
}
