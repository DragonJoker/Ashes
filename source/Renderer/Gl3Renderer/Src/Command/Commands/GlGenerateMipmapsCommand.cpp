/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "GlGenerateMipmapsCommand.hpp"

#include "Image/GlTexture.hpp"

namespace gl_renderer
{
	GenerateMipmapsCommand::GenerateMipmapsCommand( Device const & device
		, Texture const & texture )
		: CommandBase{ device }
		, m_texture{ texture }
	{
	}

	void GenerateMipmapsCommand::apply()const
	{
		glLogCommand( "GenerateMipmapsCommand" );
		glLogCall( m_device.getContext(), glBindTexture, m_texture.getTarget(), m_texture.getImage() );
		glLogCall( m_device.getContext(), glGenerateMipmap, m_texture.getTarget() );
		glLogCall( m_device.getContext(), glBindTexture, m_texture.getTarget(), 0 );
	}

	CommandPtr GenerateMipmapsCommand::clone()const
	{
		return std::make_unique< GenerateMipmapsCommand >( *this );
	}
}
