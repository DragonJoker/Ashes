/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "GlBlitImageCommand.hpp"

#include "Core/GlDevice.hpp"
#include "Image/GlTexture.hpp"
#include "Image/GlTextureView.hpp"
#include "RenderPass/GlFrameBuffer.hpp"

#include <Image/ImageSubresourceRange.hpp>
#include <RenderPass/FrameBufferAttachment.hpp>

namespace gl_renderer
{
	namespace
	{
		GlImageAspectFlags getMask( renderer::PixelFormat format )
		{
			GlImageAspectFlags result = 0u;

			if ( renderer::isDepthFormat( format ) )
			{
				result |= GL_DEPTH_BUFFER_BIT;
			}

			if ( renderer::isStencilFormat( format ) )
			{
				result |= GL_STENCIL_BUFFER_BIT;
			}

			if ( !renderer::isDepthFormat( format )
				&& !renderer::isStencilFormat( format ) )
			{
				result |= GL_COLOR_BUFFER_BIT;
			}

			return result;
		}

		GlAttachmentPoint getAttachmentPoint( renderer::PixelFormat format )
		{
			switch ( format )
			{
			case renderer::PixelFormat::eD16:
			case renderer::PixelFormat::eD32F:
				return GL_ATTACHMENT_POINT_DEPTH;

			case renderer::PixelFormat::eD24S8:
			case renderer::PixelFormat::eD32FS8:
				return GL_ATTACHMENT_POINT_DEPTH_STENCIL;

			case renderer::PixelFormat::eS8:
				return GL_ATTACHMENT_POINT_STENCIL;

			default:
				return GL_ATTACHMENT_POINT_COLOR0;
			}
		}

		GlAttachmentType getAttachmentType( renderer::PixelFormat format )
		{
			switch ( format )
			{
			case renderer::PixelFormat::eD16:
			case renderer::PixelFormat::eD32F:
				return GL_ATTACHMENT_TYPE_DEPTH;

			case renderer::PixelFormat::eD24S8:
			case renderer::PixelFormat::eD32FS8:
				return GL_ATTACHMENT_TYPE_DEPTH_STENCIL;

			case renderer::PixelFormat::eS8:
				return GL_ATTACHMENT_TYPE_STENCIL;

			default:
				return GL_ATTACHMENT_TYPE_COLOR;
			}
		}
	}

	BlitImageCommand::BlitImageCommand( Device const & device
		, renderer::Texture const & srcImage
		, renderer::Texture const & dstImage
		, std::vector< renderer::ImageBlit > const & regions
		, renderer::Filter filter )
		: m_srcTexture{ static_cast< Texture const & >( srcImage ) }
		, m_dstTexture{ static_cast< Texture const & >( dstImage ) }
		, m_regions{ regions }
		, m_srcFbo{ device.getBlitSrcFbo() }
		, m_dstFbo{ device.getBlitDstFbo() }
		, m_filter{ convert( filter ) }
		, m_mask{ getMask( m_srcTexture.getFormat() ) }
		, m_srcAttach{ getAttachmentPoint( m_srcTexture.getFormat() ), m_srcTexture.getImage(), getAttachmentType( m_srcTexture.getFormat() ) }
		, m_dstAttach{ getAttachmentPoint( m_dstTexture.getFormat() ), m_dstTexture.getImage(), getAttachmentType( m_dstTexture.getFormat() ) }
	{
	}

	BlitImageCommand::~BlitImageCommand()
	{
	}

	void BlitImageCommand::apply()const
	{
		for ( auto & region : m_regions )
		{
			// Setup source FBO
			glLogCall( gl::BindFramebuffer, GL_FRAMEBUFFER, m_srcFbo );
			glLogCall( gl::FramebufferTexture2D
				, GL_FRAMEBUFFER
				, m_srcAttach.point
				, GL_TEXTURE_2D
				, m_srcAttach.object
				, region.srcSubresource.mipLevel );

			// Setup dst FBO
			glLogCall( gl::BindFramebuffer, GL_FRAMEBUFFER, m_dstFbo );
			glLogCall( gl::FramebufferTexture2D
				, GL_FRAMEBUFFER
				, m_dstAttach.point
				, GL_TEXTURE_2D
				, m_dstAttach.object
				, region.dstSubresource.mipLevel );

			// Perform the blit
			glLogCall( gl::BindFramebuffer, GL_READ_FRAMEBUFFER, m_srcFbo );
			glLogCall( gl::BindFramebuffer, GL_DRAW_FRAMEBUFFER, m_dstFbo );
			glLogCall( gl::BlitFramebuffer
				, region.srcOffset[0]
				, region.srcOffset[1]
				, region.srcExtent[0]
				, region.srcExtent[1]
				, region.dstOffset[0]
				, region.dstOffset[1]
				, region.dstExtent[0]
				, region.dstExtent[1]
				, m_mask
				, m_filter );
			glLogCall( gl::BindFramebuffer, GL_DRAW_FRAMEBUFFER, 0u );
			glLogCall( gl::BindFramebuffer, GL_READ_FRAMEBUFFER, 0u );
		}
	}

	CommandPtr BlitImageCommand::clone()const
	{
		return std::make_unique< BlitImageCommand >( *this );
	}
}
