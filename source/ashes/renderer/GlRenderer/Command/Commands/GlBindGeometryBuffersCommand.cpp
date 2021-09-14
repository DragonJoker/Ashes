/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlBindGeometryBuffersCommand.hpp"

#include "Core/GlContextLock.hpp"

#include "Buffer/GlGeometryBuffers.hpp"

namespace ashes::gl
{
	void buildBindGeometryBuffersCommand( GeometryBuffers const & vao
		, CmdList & list )
	{
		glLogCommand( list, "BindGeometryBuffersCommand" );
		list.push_back( makeCmd< OpType::eBindVextexArray >( &vao ) );
	}
}
