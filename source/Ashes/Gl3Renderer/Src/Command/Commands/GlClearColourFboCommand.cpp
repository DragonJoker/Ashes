/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "GlClearColourFboCommand.hpp"

#include "Core/GlDevice.hpp"
#include "Image/GlTextureView.hpp"
#include "Image/GlTexture.hpp"
#include "RenderPass/GlFrameBuffer.hpp"

namespace gl_renderer
{
	ClearColourFboCommand::ClearColourFboCommand( Device const & device
		, ashes::TextureView const & image
		, ashes::ClearColorValue const & colour )
		: CommandBase{ device }
		, m_image{ static_cast< TextureView const & >( image ) }
		, m_colour{ colour }
		, m_internal{ getInternal( m_image.getFormat() ) }
		, m_format{ getFormat( m_internal ) }
		, m_type{ getType( m_internal ) }
	{
	}

	void ClearColourFboCommand::apply( ContextLock const & context )const
	{
		glLogCommand( "ClearColourFboCommand" );
		auto & image = static_cast< Texture const & >( m_image.getTexture() );
		auto target = GL_TEXTURE_2D;

		if ( image.getSamplesCount() > ashes::SampleCountFlag::e1 )
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
