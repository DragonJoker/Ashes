/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlBindGeometryBuffersCommand.hpp"

#include "Buffer/GlGeometryBuffers.hpp"

namespace ashes::gl4
{
	BindGeometryBuffersCommand::BindGeometryBuffersCommand( VkDevice device
		, GeometryBuffers const & vao )
		: CommandBase{ device }
		, m_vao{ static_cast< GeometryBuffers const & >( vao ) }
	{
	}

	void BindGeometryBuffersCommand::apply( ContextLock const & context )const
	{
		glLogCommand( "BindGeometryBuffersCommand" );
		glLogCall( context
			, glBindVertexArray
			, m_vao.getVao() );
	}

	CommandPtr BindGeometryBuffersCommand::clone()const
	{
		return std::make_unique< BindGeometryBuffersCommand >( *this );
	}
}
