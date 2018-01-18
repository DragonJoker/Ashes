/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#include "GlFrameBuffer.hpp"

#include "GlQueue.hpp"
#include "GlRenderPass.hpp"
#include "GlTexture.hpp"

namespace gl_renderer
{
	namespace
	{
		GLenum getAttachmentType( Texture const & texture
			, renderer::UInt32Array & colours )
		{
			switch ( texture.getFormat() )
			{
			case renderer::PixelFormat::eD16:
				return GL_DEPTH_ATTACHMENT;

			case renderer::PixelFormat::eD24S8:
				return GL_DEPTH_STENCIL_ATTACHMENT;

			case renderer::PixelFormat::eS8:
				return GL_STENCIL_ATTACHMENT;

			default:
				colours.push_back( uint32_t( GL_COLOR_ATTACHMENT0 + colours.size() ) );
				return GLenum( GL_COLOR_ATTACHMENT0 + colours.size() - 1u );
			}
		}
	}

	FrameBuffer::FrameBuffer( renderer::RenderPass const & renderPass
		, renderer::UIVec2 const & dimensions )
		: renderer::FrameBuffer{ renderPass, dimensions, renderer::TextureCRefArray{} }
		, m_frameBuffer{ 0u }
	{
	}

	FrameBuffer::FrameBuffer( renderer::RenderPass const & renderPass
		, renderer::UIVec2 const & dimensions
		, renderer::TextureCRefArray const & textures )
		: renderer::FrameBuffer{ renderPass, dimensions, textures }
	{
		glGenFramebuffers( 1, &m_frameBuffer );
		glBindFramebuffer( GL_FRAMEBUFFER, m_frameBuffer );
		renderer::UInt32Array colours;

		for ( auto & texture : textures )
		{
			glFramebufferTexture2D( GL_FRAMEBUFFER
				, getAttachmentType( static_cast< Texture const & >( texture.get() ), colours )
				, GL_TEXTURE_2D
				, static_cast< Texture const & >( texture.get() ).getImage()
				, 0 );
		}

		glDrawBuffers( GLsizei( colours.size() ), colours.data() );
		glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	}

	FrameBuffer::~FrameBuffer()
	{
		if ( m_frameBuffer > 0u )
		{
			glDeleteFramebuffers( 1, &m_frameBuffer );
		}
	}

	void FrameBuffer::download( renderer::Queue const & queue
		, uint32_t index
		, uint32_t xoffset
		, uint32_t yoffset
		, uint32_t width
		, uint32_t height
		, renderer::PixelFormat format
		, uint8_t * data )const noexcept
	{
		glReadPixels( xoffset
			, yoffset
			, width
			, height
			, getFormat( format )
			, getType( format )
			, data );
	}
}
