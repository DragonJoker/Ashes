/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlBindGeometryBuffersCommand.hpp"

#include "Core/GlContextLock.hpp"

#include "Buffer/GlGeometryBuffers.hpp"

namespace ashes::gl3
{
	void apply( ContextLock const & context
		, CmdBindVextexArray const & cmd )
	{
		if ( cmd.vao )
		{
			glLogCall( context
				, glBindVertexArray
				, cmd.vao->getVao() );
		}
		else
		{
			glLogCall( context
				, glBindVertexArray
				, 0u );
		}
	}

	void buildBindGeometryBuffersCommand( GeometryBuffers const & vao
		, CmdList & list )
	{
		glLogCommand( "BindGeometryBuffersCommand" );
		list.push_back( makeCmd< OpType::eBindVextexArray >( &vao ) );
	}
}
