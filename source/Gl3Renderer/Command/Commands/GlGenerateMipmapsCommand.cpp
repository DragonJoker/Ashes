/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlGenerateMipmapsCommand.hpp"

#include "Image/GlTexture.hpp"

namespace gl_renderer
{
	GenerateMipmapsCommand::GenerateMipmapsCommand( Device const & device
		, Texture const & texture )
		: CommandBase{ device }
		, m_texture{ texture }
	{
	}

	void GenerateMipmapsCommand::apply( ContextLock const & context )const
	{
		glLogCommand( "GenerateMipmapsCommand" );
		glLogCall( context
			, glBindTexture
			, m_texture.getTarget()
			, m_texture.getImage() );
		glLogCall( context
			, glGenerateMipmap
			, m_texture.getTarget() );
		glLogCall( context
			, glBindTexture
			, m_texture.getTarget()
			, 0 );
	}

	CommandPtr GenerateMipmapsCommand::clone()const
	{
		return std::make_unique< GenerateMipmapsCommand >( *this );
	}
}
