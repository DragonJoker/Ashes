/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlClearDepthStencilFboCommand.hpp"

#include "Core/GlDevice.hpp"
#include "Image/GlImageView.hpp"
#include "Image/GlImage.hpp"
#include "RenderPass/GlFrameBuffer.hpp"

namespace gl_renderer
{
	ClearDepthStencilFboCommand::ClearDepthStencilFboCommand( Device const & device
		, ashes::ImageView const & image
		, ashes::DepthStencilClearValue const & value )
		: CommandBase{ device }
		, m_image{ static_cast< ImageView const & >( image ) }
		, m_value{ value }
		, m_internal{ getInternal( m_image.getFormat() ) }
		, m_format{ getFormat( m_internal ) }
		, m_type{ getType( m_internal ) }
	{
	}

	void ClearDepthStencilFboCommand::apply( ContextLock const & context )const
	{
		glLogCommand( "ClearDepthStencilFboCommand" );
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
		glLogCall( context
			, glFramebufferTexture2D
			, GL_FRAMEBUFFER
			, getAttachmentPoint( m_image )
			, target
			, image.getImage()
			, m_image.getSubResourceRange().baseMipLevel );
		glLogCall( context
			, glDepthMask
			, GL_TRUE );
		auto stencil = GLint( m_value.stencil );

		if ( isDepthStencilFormat( m_internal ) )
		{
			glLogCall( context
				, glClearBufferfi
				, GL_CLEAR_TARGET_DEPTH_STENCIL
				, 0u
				, m_value.depth
				, stencil );
		}
		else if ( isDepthFormat( m_internal ) )
		{
			glLogCall( context
				, glClearBufferfv
				, GL_CLEAR_TARGET_DEPTH
				, 0u
				, &m_value.depth );
		}
		else if ( isStencilFormat( m_internal ) )
		{
			glLogCall( context
				, glClearBufferiv
				, GL_CLEAR_TARGET_STENCIL
				, 0u
				, &stencil );
		}

		glLogCall( context
			, glBindFramebuffer
			, GL_FRAMEBUFFER
			, m_device.getCurrentFramebuffer() );
	}

	CommandPtr ClearDepthStencilFboCommand::clone()const
	{
		return std::make_unique< ClearDepthStencilFboCommand >( *this );
	}
}
