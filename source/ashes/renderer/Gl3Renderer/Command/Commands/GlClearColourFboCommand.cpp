/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlClearColourFboCommand.hpp"

#include "Core/GlDevice.hpp"
#include "Image/GlImageView.hpp"
#include "Image/GlImage.hpp"
#include "RenderPass/GlFrameBuffer.hpp"

namespace gl_renderer
{
	ClearColourFboCommand::ClearColourFboCommand( Device const & device
		, ashes::ImageView const & image
		, VkClearColorValue const & colour )
		: CommandBase{ device }
		, m_image{ static_cast< ImageView const & >( image ) }
		, m_colour{ colour }
		, m_internal{ getInternal( m_image.getFormat() ) }
		, m_format{ getFormat( m_internal ) }
		, m_type{ getType( m_internal ) }
	{
	}

	void ClearColourFboCommand::apply( ContextLock const & context )const
	{
		glLogCommand( "ClearColourFboCommand" );
		auto & image = static_cast< Image const & >( m_image.getImage() );
		auto target = GL_TEXTURE_2D;

		if ( image.getSamplesCount() > VK_SAMPLE_COUNT_1_BIT )
		{
			target = GL_TEXTURE_2D_MULTISAMPLE;
		}

		glLogCall( context
			, glBindFramebuffer
			, GL_FRAMEBUFFER
			, m_device.getBlitDstFbo() );
		GLenum point = getAttachmentPoint( m_image );
		glLogCall( context
			, glFramebufferTexture2D
			, GL_FRAMEBUFFER
			, point
			, target
			, image.getImage()
			, m_image.getSubResourceRange().baseMipLevel );
		glLogCall( context
			, glDrawBuffers
			, 1
			, &point );
		glLogCall( context
			, glClearBufferfv
			, GL_CLEAR_TARGET_COLOR
			, 0u
			, m_colour.float32.data() );
		glLogCall( context
			, glBindFramebuffer
			, GL_FRAMEBUFFER
			, m_device.getCurrentFramebuffer() );
	}

	CommandPtr ClearColourFboCommand::clone()const
	{
		return std::make_unique< ClearColourFboCommand >( *this );
	}
}
