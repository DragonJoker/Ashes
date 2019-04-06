/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlClearColourCommand.hpp"

#include "Image/GlImageView.hpp"
#include "Image/GlImage.hpp"

namespace gl_renderer
{
	ClearColourCommand::ClearColourCommand( Device const & device
		, ashes::ImageView const & image
		, VkClearColorValue const & colour )
		: CommandBase{ device }
		, m_image{ static_cast< Image const & >( image.getImage() ) }
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
