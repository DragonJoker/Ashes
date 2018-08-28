/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "GlClearDepthStencilCommand.hpp"

#include "Image/GlTexture.hpp"
#include "Image/GlTextureView.hpp"

namespace gl_renderer
{
	ClearDepthStencilCommand::ClearDepthStencilCommand( Device const & device
		, renderer::TextureView const & image
		, renderer::DepthStencilClearValue const & value )
		: CommandBase{ device }
		, m_image{ static_cast< Texture const & >( image.getTexture() ) }
		, m_value{ value }
		, m_internal{ getInternal( m_image.getFormat() ) }
		, m_format{ getFormat( m_internal ) }
		, m_type{ getType( m_internal ) }
	{
	}

	void ClearDepthStencilCommand::apply()const
	{
		glLogCommand( "ClearDepthStencilCommand" );

		if ( isDepthStencilFormat( m_internal ) )
		{
			glLogCall( m_device.getContext(), glClearTexImage_ARB
				, m_image.getImage()
				, 0
				, m_format
				, m_type
				, &m_value.depth );
		}
		else if ( isStencilFormat( m_internal ) )
		{
			glLogCall( m_device.getContext(), glClearTexImage_ARB
				, m_image.getImage()
				, 0
				, m_format
				, m_type
				, &m_value.stencil );
		}
		else if ( isDepthFormat( m_internal ) )
		{
			glLogCall( m_device.getContext(), glClearTexImage_ARB
				, m_image.getImage()
				, 0
				, m_format
				, m_type
				, &m_value.depth );
		}
	}

	CommandPtr ClearDepthStencilCommand::clone()const
	{
		return std::make_unique< ClearDepthStencilCommand >( *this );
	}
}
