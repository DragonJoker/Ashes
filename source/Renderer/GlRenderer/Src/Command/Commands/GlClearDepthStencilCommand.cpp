/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "GlClearDepthStencilCommand.hpp"

#include "Image/GlTextureView.hpp"

namespace gl_renderer
{
	ClearDepthStencilCommand::ClearDepthStencilCommand( Device const & device
		, renderer::TextureView const & image
		, renderer::DepthStencilClearValue const & value )
		: CommandBase{ device }
		, m_image{ static_cast< TextureView const & >( image ) }
		, m_value{ value }
		, m_internal{ getInternal( m_image.getFormat() ) }
		, m_format{ getFormat( m_internal ) }
		, m_type{ getType( m_internal ) }
	{
	}

	void ClearDepthStencilCommand::apply()const
	{
		glLogCommand( "ClearDepthStencilCommand" );

		if ( m_device.getContext().glClearTexImage )
		{
			if ( renderer::isDepthStencilFormat( m_image.getFormat() ) )
			{
				glLogCall( m_device.getContext(), glClearTexImage
					, m_image.getImage()
					, 0
					, m_format
					, m_type
					, &m_value.depth );
			}
			else if ( renderer::isStencilFormat( m_image.getFormat() ) )
			{
				glLogCall( m_device.getContext(), glClearTexImage
					, m_image.getImage()
					, 0
					, m_format
					, m_type
					, &m_value.stencil );
			}
			else if ( renderer::isDepthFormat( m_image.getFormat() ) )
			{
				glLogCall( m_device.getContext(), glClearTexImage
					, m_image.getImage()
					, 0
					, m_format
					, m_type
					, &m_value.depth );
			}
		}
		else
		{
			renderer::Logger::logError( "Unsupported command : ClearDepthStencilCommand" );
		}
	}

	CommandPtr ClearDepthStencilCommand::clone()const
	{
		return std::make_unique< ClearDepthStencilCommand >( *this );
	}
}
