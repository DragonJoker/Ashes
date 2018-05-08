/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#include "RenderPass/GlFrameBuffer.hpp"

#include "Command/GlQueue.hpp"
#include "RenderPass/GlRenderPass.hpp"
#include "Image/GlTexture.hpp"
#include "Image/GlTextureView.hpp"

#include <RenderPass/AttachmentReference.hpp>

#include <iostream>

namespace gl_renderer
{
	namespace
	{
		enum GlFramebufferStatus
			: GLenum
		{
			GL_FRAMEBUFFER_STATUS_UNDEFINED = 0x8219,
			GL_FRAMEBUFFER_STATUS_COMPLETE = 0x8CD5,
			GL_FRAMEBUFFER_STATUS_INCOMPLETE_ATTACHMENT = 0x8CD6,
			GL_FRAMEBUFFER_STATUS_INCOMPLETE_MISSING_ATTACHMENT = 0x8CD7,
			GL_FRAMEBUFFER_STATUS_INCOMPLETE_DRAW_BUFFER = 0x8CDB,
			GL_FRAMEBUFFER_STATUS_INCOMPLETE_READ_BUFFER = 0x8CDC,
			GL_FRAMEBUFFER_STATUS_UNSUPPORTED = 0x8CDD,
			GL_FRAMEBUFFER_STATUS_INCOMPLETE_MULTISAMPLE = 0x8D56,
			GL_FRAMEBUFFER_STATUS_INCOMPLETE_LAYER_TARGETS = 0x8DA8,
		};

		GlAttachmentPoint getAttachmentPoint( renderer::Format format )
		{
			if ( renderer::isDepthStencilFormat( format ) )
			{
				return GL_ATTACHMENT_POINT_DEPTH_STENCIL;
			}

			if ( renderer::isStencilFormat( format ) )
			{
				return GL_ATTACHMENT_POINT_STENCIL;
			}

			if ( renderer::isDepthFormat( format ) )
			{
				return GL_ATTACHMENT_POINT_DEPTH;
			}

			return GL_ATTACHMENT_POINT_COLOR0;
		}

		GlAttachmentPoint getAttachmentPoint( TextureView const & texture )
		{
			return getAttachmentPoint( texture.getFormat() );
		}

		GlAttachmentType getAttachmentType( renderer::Format format )
		{
			if ( renderer::isDepthStencilFormat( format ) )
			{
				return GL_ATTACHMENT_TYPE_DEPTH_STENCIL;
			}

			if ( renderer::isStencilFormat( format ) )
			{
				return GL_ATTACHMENT_TYPE_STENCIL;
			}

			if ( renderer::isDepthFormat( format ) )
			{
				return GL_ATTACHMENT_TYPE_DEPTH;
			}

			return GL_ATTACHMENT_TYPE_COLOR;
		}

		GlAttachmentType getAttachmentType( TextureView const & texture )
		{
			return getAttachmentType( texture.getFormat() );
		}

		void doCheck( GLenum status )
		{
			switch ( status )
			{
			case 0:
				renderer::Logger::logError( "An error has occured." );
				assert( false );
				break;

			case GL_FRAMEBUFFER_STATUS_COMPLETE:
				break;

			case GL_FRAMEBUFFER_STATUS_UNDEFINED:
				renderer::Logger::logError( "The specified framebuffer is the default read or draw framebuffer, but the default framebuffer does not exist." );
				assert( false );
				break;

			case GL_FRAMEBUFFER_STATUS_INCOMPLETE_ATTACHMENT:
				renderer::Logger::logError( "At least one of the framebuffer attachment points are framebuffer incomplete." );
				assert( false );
				break;

			case GL_FRAMEBUFFER_STATUS_INCOMPLETE_MISSING_ATTACHMENT:
				renderer::Logger::logError( "The framebuffer does not have at least one image attached to it." );
				assert( false );
				break;

			case GL_FRAMEBUFFER_STATUS_INCOMPLETE_DRAW_BUFFER:
				renderer::Logger::logError( "The value of GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE is GL_NONE for any color attachment point(s) named by GL_DRAW_BUFFERi." );
				assert( false );
				break;

			case GL_FRAMEBUFFER_STATUS_INCOMPLETE_READ_BUFFER:
				renderer::Logger::logError( "GL_READ_BUFFER is not GL_NONE and the value of GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE is GL_NONE for the color attachment point named by GL_READ_BUFFER." );
				assert( false );
				break;

			case GL_FRAMEBUFFER_STATUS_UNSUPPORTED:
				renderer::Logger::logError( "The combination of internal formats of the attached images violates an implementation-dependent set of restrictions." );
				assert( false );
				break;

			case GL_FRAMEBUFFER_STATUS_INCOMPLETE_MULTISAMPLE:
				renderer::Logger::logError( "One of the following:" );
				renderer::Logger::logError( "  - The value of GL_RENDERBUFFER_SAMPLES is not the same for all attached renderbuffers;" );
				renderer::Logger::logError( "  - The value of GL_TEXTURE_SAMPLES is the not same for all attached textures;" );
				renderer::Logger::logError( "  - The attached images are a mix of renderbuffers and textures, the value of GL_RENDERBUFFER_SAMPLES does not match the value of GL_TEXTURE_SAMPLES;" );
				renderer::Logger::logError( "  - The value of GL_TEXTURE_FIXED_SAMPLE_LOCATIONS is not the same for all attached textures;" );
				renderer::Logger::logError( "  - The attached images are a mix of renderbuffers and textures, the value of GL_TEXTURE_FIXED_SAMPLE_LOCATIONS is not GL_TRUE for all attached textures." );
				assert( false );
				break;

			case GL_FRAMEBUFFER_STATUS_INCOMPLETE_LAYER_TARGETS:
				renderer::Logger::logError( "At least one framebuffer attachment is layered, and any populated attachment is not layered, or all populated color attachments are not from textures of the same target." );
				assert( false );
				break;
			}
		}
	}

	FrameBuffer::FrameBuffer( RenderPass const & renderPass
		, renderer::Extent2D const & dimensions )
		: renderer::FrameBuffer{ renderPass, dimensions, renderer::FrameBufferAttachmentArray{} }
		, m_frameBuffer{ 0u }
		, m_renderPass{ renderPass }
	{
	}

	FrameBuffer::FrameBuffer( RenderPass const & renderPass
		, renderer::Extent2D const & dimensions
		, renderer::FrameBufferAttachmentArray && views )
		: renderer::FrameBuffer{ renderPass, dimensions, std::move( views ) }
		, m_renderPass{ renderPass }
	{
		glLogCall( gl::GenFramebuffers, 1, &m_frameBuffer );
		glLogCall( gl::BindFramebuffer, GL_FRAMEBUFFER, m_frameBuffer );

		for ( auto & attach : m_attachments )
		{
			auto & glview = static_cast< TextureView const & >( attach.getView() );
			auto & gltexture = static_cast< Texture const & >( glview.getTexture() );

			// If the image doesn't exist, it means it is a backbuffer image, hence ignore the attachment.
			if ( gltexture.hasImage() )
			{
				uint32_t index = m_renderPass.getAttachmentIndex( attach.getAttachment() );
				auto image = glview.getImage();
				auto mipLevel = glview.getSubResourceRange().baseMipLevel;

				if ( glview.getSubResourceRange().baseMipLevel )
				{
					if ( gltexture.getLayerCount() == 1u )
					{
						image = gltexture.getImage();
					}
					else
					{
						mipLevel = 0u;
					}
				}

				Attachment attachment
				{
					getAttachmentPoint( glview ),
					image,
					getAttachmentType( glview ),
				};

				if ( attachment.point == GL_ATTACHMENT_POINT_DEPTH_STENCIL
					|| attachment.point == GL_ATTACHMENT_POINT_DEPTH
					|| attachment.point == GL_ATTACHMENT_POINT_STENCIL )
				{
					m_depthStencilAttach = attachment;
				}
				else
				{
					m_colourAttaches.push_back( attachment );
				}

				m_allAttaches.push_back( attachment );
				auto target = GL_TEXTURE_2D;

				if ( gltexture.getSamplesCount() > renderer::SampleCountFlag::e1 )
				{
					target = GL_TEXTURE_2D_MULTISAMPLE;
				}

				glLogCall( gl::FramebufferTexture2D
					, GL_FRAMEBUFFER
					, GlAttachmentPoint( attachment.point + index )
					, target
					, attachment.object
					, mipLevel );
				doCheck( gl::CheckFramebufferStatus( GL_FRAMEBUFFER ) );
			}
			else
			{
				Attachment attachment
				{
					GL_ATTACHMENT_POINT_BACK,
					GL_INVALID_INDEX,
					getAttachmentType( attach.getView().getFormat() )
				};

				if ( renderer::isDepthOrStencilFormat( attach.getFormat() ) )
				{
					m_depthStencilAttach = attachment;
				}
				else
				{
					m_colourAttaches.push_back( attachment );
				}

				m_allAttaches.push_back( attachment );
			}
		}

		doCheck( gl::CheckFramebufferStatus( GL_FRAMEBUFFER ) );
		glLogCall( gl::BindFramebuffer, GL_FRAMEBUFFER, 0 );
	}

	FrameBuffer::~FrameBuffer()
	{
		if ( m_frameBuffer > 0u )
		{
			glLogCall( gl::DeleteFramebuffers, 1, &m_frameBuffer );
		}
	}

	void FrameBuffer::setDrawBuffers( AttachmentDescriptionArray const & attaches )const
	{
		m_drawBuffers.clear();

		for ( auto & attach : attaches )
		{
			auto & fboAttach = m_attachments[attach.index];

			if ( static_cast< Texture const & >( fboAttach.getTexture() ).hasImage() )
			{
				m_drawBuffers.push_back( getAttachmentPoint( attach.attach.get().format ) + attach.index );
			}
			else if ( attaches.size() == 1 )
			{
				m_drawBuffers.push_back( GL_ATTACHMENT_POINT_BACK );
			}
		}

		glLogCall( gl::DrawBuffers, GLsizei( m_drawBuffers.size() ), m_drawBuffers.data() );
	}

	void FrameBuffer::setDrawBuffers( renderer::AttachmentReferenceArray const & attaches )const
	{
		if ( getFrameBuffer() != GL_INVALID_INDEX )
		{
			renderer::UInt32Array colours;

			if ( m_colourAttaches.empty() && attaches.size() == 1 )
			{
				colours.push_back( GL_ATTACHMENT_POINT_BACK );
			}
			else
			{
				for ( auto & attach : attaches )
				{
					auto & fboAttach = m_colourAttaches[attach.attachment];
					colours.push_back( fboAttach.point + attach.attachment );
				}
			}

			if ( m_drawBuffers != colours )
			{
				m_drawBuffers = colours;
				glLogCall( gl::DrawBuffers, GLsizei( m_drawBuffers.size() ), m_drawBuffers.data() );
			}
		}
	}
}
