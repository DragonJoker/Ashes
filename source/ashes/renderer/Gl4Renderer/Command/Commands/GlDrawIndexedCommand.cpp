/*
This file belongs to Ashes.
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

	void apply( ContextLock const & context
		, CmdDrawIndexed const & cmd )
	{
		glLogCall( context
			, glDrawElementsInstancedBaseVertexBaseInstance
			, cmd.mode
			, cmd.indexCount
			, cmd.type
			, getBufferOffset( cmd.indexOffset )
			, cmd.instCount
			, cmd.vertexOffset
			, cmd.firstInstance );
	}

	void buildDrawIndexedCommand( uint32_t indexCount
		, uint32_t instCount
		, uint32_t firstIndex
		, uint32_t vertexOffset
		, uint32_t firstInstance
		, VkPrimitiveTopology mode
		, VkIndexType type
		, CmdList & list )
	{
		glLogCommand( "DrawIndexedCommand" );
		list.push_back( makeCmd< OpType::eDrawIndexed >( indexCount
			, instCount
			, firstIndex * getSize( type )
			, vertexOffset
			, firstInstance
			, convert( mode )
			, convert( type ) ) );
	}
}
