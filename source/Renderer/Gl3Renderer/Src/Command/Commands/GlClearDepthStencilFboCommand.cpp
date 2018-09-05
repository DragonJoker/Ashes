/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "GlClearDepthStencilFboCommand.hpp"

#include "Core/GlDevice.hpp"
#include "Image/GlTextureView.hpp"
#include "Image/GlTexture.hpp"
#include "RenderPass/GlFrameBuffer.hpp"

namespace gl_renderer
{
	ClearDepthStencilFboCommand::ClearDepthStencilFboCommand( Device const & device
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

	void ClearDepthStencilFboCommand::apply( ContextLock const & context )const
	{
		glLogCommand( "ClearDepthStencilFboCommand" );
		auto & image = static_cast< Texture const & >( m_image.getTexture() );
		auto target = GL_TEXTURE_2D;

		if ( image.getSamplesCount() > renderer::SampleCountFlag::e1 )
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
			, 0u );
	}

	CommandPtr ClearDepthStencilFboCommand::clone()const
	{
		return std::make_unique< ClearDepthStencilFboCommand >( *this );
	}
}
