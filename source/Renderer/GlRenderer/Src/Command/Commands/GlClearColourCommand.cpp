/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "GlClearColourCommand.hpp"

#include "Image/GlTextureView.hpp"

namespace gl_renderer
{
	ClearColourCommand::ClearColourCommand( Device const & device
		, renderer::TextureView const & image
		, renderer::ClearColorValue const & colour )
		: CommandBase{ device }
		, m_image{ static_cast< TextureView const & >( image ) }
		, m_colour{ colour }
		, m_internal{ getInternal( m_image.getFormat() ) }
		, m_format{ getFormat( m_internal ) }
		, m_type{ getType( m_internal ) }
	{
	}

	void ClearColourCommand::apply()const
	{
		glLogCommand( "ClearColourCommand" );

		if ( m_device.getContext().hasClearTexImage() )
		{
			glLogCall( m_device.getContext(), glClearTexImage
				, m_image.getImage()
				, 0
				, m_format
				, m_type
				, m_colour.float32.data() );
		}
		else
		{
			renderer::Logger::logError( "Unsupported command : ClearColourCommand" );
		}
	}

	CommandPtr ClearColourCommand::clone()const
	{
		return std::make_unique< ClearColourCommand >( *this );
	}
}
