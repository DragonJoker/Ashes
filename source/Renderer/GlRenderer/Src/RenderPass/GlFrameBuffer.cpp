/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#include "RenderPass/GlFrameBuffer.hpp"

#include "Command/GlQueue.hpp"
#include "RenderPass/GlRenderPass.hpp"
#include "Image/GlTexture.hpp"

#include <iostream>

namespace gl_renderer
{
	namespace
	{
		GLenum getAttachmentPoint( Texture const & texture )
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
				return GL_COLOR_ATTACHMENT0;
			}
		}

		GLenum getAttachmentType( Texture const & texture )
		{
			switch ( texture.getFormat() )
			{
			case renderer::PixelFormat::eD16:
				return GL_DEPTH;

			case renderer::PixelFormat::eD24S8:
				return GL_DEPTH_STENCIL;

			case renderer::PixelFormat::eS8:
				return GL_STENCIL;

			default:
				return GL_COLOR;
			}
		}

		void doCheck( GLenum status )
		{
			switch ( status )
			{
			case 0:
				std::cerr << "An error has occured." << std::endl;
				assert( false );
				break;

			case GL_FRAMEBUFFER_COMPLETE:
				break;

			case GL_FRAMEBUFFER_UNDEFINED:
				std::cerr << "The specified framebuffer is the default read or draw framebuffer, but the default framebuffer does not exist." << std::endl;
				assert( false );
				break;

			case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
				std::cerr << "At least one of the framebuffer attachment points are framebuffer incomplete." << std::endl;
				assert( false );
				break;

			case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
				std::cerr << "The framebuffer does not have at least one image attached to it." << std::endl;
				assert( false );
				break;

			case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
				std::cerr << "The value of GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE is GL_NONE for any color attachment point(s) named by GL_DRAW_BUFFERi." << std::endl;
				assert( false );
				break;

			case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
				std::cerr << "GL_READ_BUFFER is not GL_NONE and the value of GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE is GL_NONE for the color attachment point named by GL_READ_BUFFER." << std::endl;
				assert( false );
				break;

			case GL_FRAMEBUFFER_UNSUPPORTED:
				std::cerr << "The combination of internal formats of the attached images violates an implementation-dependent set of restrictions." << std::endl;
				assert( false );
				break;

			case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
				std::cerr << "One of the following:" << std::endl;
				std::cerr << "  - The value of GL_RENDERBUFFER_SAMPLES is not the same for all attached renderbuffers;" << std::endl;
				std::cerr << "  - The value of GL_TEXTURE_SAMPLES is the not same for all attached textures;" << std::endl;
				std::cerr << "  - The attached images are a mix of renderbuffers and textures, the value of GL_RENDERBUFFER_SAMPLES does not match the value of GL_TEXTURE_SAMPLES;" << std::endl;
				std::cerr << "  - The value of GL_TEXTURE_FIXED_SAMPLE_LOCATIONS is not the same for all attached textures;" << std::endl;
				std::cerr << "  - The attached images are a mix of renderbuffers and textures, the value of GL_TEXTURE_FIXED_SAMPLE_LOCATIONS is not GL_TRUE for all attached textures." << std::endl;
				assert( false );
				break;

			case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
				std::cerr << "At least one framebuffer attachment is layered, and any populated attachment is not layered, or all populated color attachments are not from textures of the same target." << std::endl;
				assert( false );
				break;
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
			Attachment attachment
			{
				getAttachmentPoint( static_cast< Texture const & >( texture.get() ) ),
				0u,
				static_cast< Texture const & >( texture.get() ).getImage(),
				getAttachmentType( static_cast< Texture const & >( texture.get() ) ),
			};

			if ( attachment.point == GL_DEPTH_STENCIL_ATTACHMENT
				|| attachment.point == GL_DEPTH_ATTACHMENT
				|| attachment.point == GL_STENCIL_ATTACHMENT )
			{
				m_depthStencilAttaches.push_back( attachment );
			}
			else
			{
				attachment.index = GLuint( m_colourAttaches.size() );
				m_colourAttaches.push_back( attachment );
				colours.push_back( attachment.point + attachment.index );
			}

			glFramebufferTexture2D( GL_FRAMEBUFFER
				, attachment.point
				, GL_TEXTURE_2D
				, attachment.object
				, 0 );
			doCheck( glCheckFramebufferStatus( GL_FRAMEBUFFER ) );
		}

		doCheck( glCheckFramebufferStatus( GL_FRAMEBUFFER ) );
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
		glBindFramebuffer( GL_FRAMEBUFFER, m_frameBuffer );
		glReadBuffer( m_colourAttaches[index].point );
		glReadPixels( xoffset
			, yoffset
			, width
			, height
			, getFormat( format )
			, getType( format )
			, data );
		glBindFramebuffer( GL_FRAMEBUFFER, 0u );
	}
}
