/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "GlClearColourCommand.hpp"

#include "Image/GlTextureView.hpp"
#include "Image/GlTexture.hpp"

namespace gl_renderer
{
	ClearColourCommand::ClearColourCommand( Device const & device
		, ashes::TextureView const & image
		, ashes::ClearColorValue const & colour )
		: CommandBase{ device }
		, m_image{ static_cast< Texture const & >( image.getTexture() ) }
		, m_colour{ colour }
		, m_internal{ getInternal( m_image.getFormat() ) }
		, m_format{ getFormat( m_internal ) }
		, m_type{ getType( m_internal ) }
	{
	}

	void ClearColourCommand::apply( ContextLock const & context )const
	{
		glLogCommand( "ClearColourCommand" );
		glLogCall( context
			, glClearTexImage_ARB
			, m_image.getImage()
			, 0
			, m_format
			, m_type
			, m_colour.float32.data() );
	}

	CommandPtr ClearColourCommand::clone()const
	{
		return std::make_unique< ClearColourCommand >( *this );
	}
}
