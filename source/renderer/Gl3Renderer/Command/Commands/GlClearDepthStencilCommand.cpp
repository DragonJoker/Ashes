/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlClearDepthStencilCommand.hpp"

#include "Image/GlImage.hpp"
#include "Image/GlImageView.hpp"

namespace gl_renderer
{
	ClearDepthStencilCommand::ClearDepthStencilCommand( Device const & device
		, ashes::ImageView const & image
		, ashes::DepthStencilClearValue const & value )
		: CommandBase{ device }
		, m_image{ static_cast< Image const & >( image.getImage() ) }
		, m_value{ value }
		, m_internal{ getInternal( m_image.getFormat() ) }
		, m_format{ getFormat( m_internal ) }
		, m_type{ getType( m_internal ) }
	{
	}

	void ClearDepthStencilCommand::apply( ContextLock const & context )const
	{
		glLogCommand( "ClearDepthStencilCommand" );

		if ( isDepthStencilFormat( m_internal ) )
		{
			glLogCall( context
				, glClearTexImage_ARB
				, m_image.getImage()
				, 0
				, m_format
				, m_type
				, &m_value.depth );
		}
		else if ( isStencilFormat( m_internal ) )
		{
			glLogCall( context
				, glClearTexImage_ARB
				, m_image.getImage()
				, 0
				, m_format
				, m_type
				, &m_value.stencil );
		}
		else if ( isDepthFormat( m_internal ) )
		{
			glLogCall( context
				, glClearTexImage_ARB
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
