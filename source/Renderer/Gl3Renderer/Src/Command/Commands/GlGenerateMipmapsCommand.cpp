/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "GlGenerateMipmapsCommand.hpp"

#include "Image/GlTexture.hpp"

namespace gl_renderer
{
	GenerateMipmapsCommand::GenerateMipmapsCommand( Texture const & texture )
		: m_texture{ texture }
	{
	}

	void GenerateMipmapsCommand::apply()const
	{
		glLogCommand( "GenerateMipmapsCommand" );
		glLogCall( gl::BindTexture, m_texture.getTarget(), m_texture.getImage() );
		glLogCall( gl::GenerateMipmap, m_texture.getTarget() );
		glLogCall( gl::BindTexture, m_texture.getTarget(), 0 );
	}

	CommandPtr GenerateMipmapsCommand::clone()const
	{
		return std::make_unique< GenerateMipmapsCommand >( *this );
	}
}
