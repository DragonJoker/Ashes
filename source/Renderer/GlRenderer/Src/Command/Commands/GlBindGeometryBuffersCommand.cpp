/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "GlBindGeometryBuffersCommand.hpp"

#include "Buffer/GlGeometryBuffers.hpp"

namespace gl_renderer
{
	BindGeometryBuffersCommand::BindGeometryBuffersCommand( Device const & device
		, GeometryBuffers const & vao )
		: CommandBase{ device }
		, m_vao{ static_cast< GeometryBuffers const & >( vao ) }
	{
	}

	void BindGeometryBuffersCommand::apply()const
	{
		glLogCommand( "BindGeometryBuffersCommand" );
		glLogCall( m_device.getContext(), glBindVertexArray, m_vao.getVao() );
	}

	CommandPtr BindGeometryBuffersCommand::clone()const
	{
		return std::make_unique< BindGeometryBuffersCommand >( *this );
	}
}
