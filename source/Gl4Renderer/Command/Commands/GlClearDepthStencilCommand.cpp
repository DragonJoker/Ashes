/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlClearDepthStencilCommand.hpp"

#include "Image/GlImageView.hpp"

namespace gl_renderer
{
	ClearDepthStencilCommand::ClearDepthStencilCommand( Device const & device
		, ashes::ImageView const & image
		, ashes::DepthStencilClearValue const & value )
		: CommandBase{ device }
		, m_image{ static_cast< ImageView const & >( image ) }
		, m_value{ value }
		, m_internal{ getInternalFormat( m_image.getFormat() ) }
		, m_format{ getFormat( m_internal ) }
		, m_type{ getType( m_internal ) }
	{
	}

	void ClearDepthStencilCommand::apply( ContextLock const & context )const
	{
		glLogCommand( "ClearDepthStencilCommand" );

		if ( context->hasClearTexImage() )
		{
			if ( ashes::isDepthStencilFormat( m_image.getFormat() ) )
			{
				glLogCall( context
					, glClearTexImage
					, m_image.getInternal()
					, 0
					, m_format
					, m_type
					, &m_value.depth );
			}
			else if ( ashes::isStencilFormat( m_image.getFormat() ) )
			{
				glLogCall( context
					, glClearTexImage
					, m_image.getInternal()
					, 0
					, m_format
					, m_type
					, &m_value.stencil );
			}
			else if ( ashes::isDepthFormat( m_image.getFormat() ) )
			{
				glLogCall( context
					, glClearTexImage
					, m_image.getInternal()
					, 0
					, m_format
					, m_type
					, &m_value.depth );
			}
		}
		else
		{
			ashes::Logger::logError( "Unsupported command : ClearDepthStencilCommand" );
		}
	}

	CommandPtr ClearDepthStencilCommand::clone()const
	{
		return std::make_unique< ClearDepthStencilCommand >( *this );
	}
}
