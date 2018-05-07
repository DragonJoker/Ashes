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
		: m_device{ device }
		, m_image{ static_cast< TextureView const & >( image ) }
		, m_value{ value }
		, m_internal{ getInternal( m_image.getFormat() ) }
		, m_format{ getFormat( m_internal ) }
		, m_type{ getType( m_internal ) }
	{
	}

	void ClearDepthStencilFboCommand::apply()const
	{
		glLogCommand( "ClearDepthStencilFboCommand" );
		auto & image = static_cast< Texture const & >( m_image.getTexture() );
		auto target = GL_TEXTURE_2D;

		if ( image.getSamplesCount() > renderer::SampleCountFlag::e1 )
		{
			target = GL_TEXTURE_2D_MULTISAMPLE;
		}

		glLogCall( gl::BindFramebuffer
			, GL_FRAMEBUFFER
			, m_device.getBlitDstFbo() );
		glLogCall( gl::FramebufferTexture2D
			, GL_FRAMEBUFFER
			, getAttachmentPoint( m_image )
			, target
			, image.getImage()
			, m_image.getSubResourceRange().baseMipLevel );
		glLogCall( gl::DepthMask, GL_TRUE );
		auto stencil = GLint( m_value.stencil );

		if ( renderer::isDepthStencilFormat( m_image.getFormat() ) )
		{
			glLogCall( gl::ClearBufferfi, GL_CLEAR_TARGET_DEPTH_STENCIL, 0u, m_value.depth, stencil );
		}
		else if ( renderer::isDepthFormat( m_image.getFormat() ) )
		{
			glLogCall( gl::ClearBufferfv, GL_CLEAR_TARGET_DEPTH, 0u, &m_value.depth );
		}
		else if ( renderer::isStencilFormat( m_image.getFormat() ) )
		{
			glLogCall( gl::ClearBufferiv, GL_CLEAR_TARGET_STENCIL, 0u, &stencil );
		}

		glLogCall( gl::BindFramebuffer
			, GL_FRAMEBUFFER
			, 0u );
	}

	CommandPtr ClearDepthStencilFboCommand::clone()const
	{
		return std::make_unique< ClearDepthStencilFboCommand >( *this );
	}
}
