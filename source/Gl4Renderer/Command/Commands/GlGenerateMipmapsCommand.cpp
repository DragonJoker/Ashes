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
		, m_texture{ texture.getImage() }
		, m_target{ convert( texture.getType(), texture.getLayerCount() ) }
	{
	}

	void GenerateMipmapsCommand::apply( ContextLock const & context )const
	{
		glLogCommand( "GenerateMipmapsCommand" );
		glLogCall( context
			, glBindTexture
			, m_target
			, m_texture );
		glLogCall( context
			, glGenerateMipmap
			, m_target );
		glLogCall( context
			, glBindTexture
			, m_target
			, 0u );
	}

	CommandPtr GenerateMipmapsCommand::clone()const
	{
		return std::make_unique< GenerateMipmapsCommand >( *this );
	}
}
