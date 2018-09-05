/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#include "RenderPass/GlFrameBuffer.hpp"

#include "Command/GlQueue.hpp"
#include "Core/GlDevice.hpp"
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
	}

	bool isSRGBFormat( renderer::Format format )
	{
		return format == renderer::Format::eR8G8_SRGB
			|| format == renderer::Format::eR8G8B8_SRGB
			|| format == renderer::Format::eB8G8R8_SRGB
			|| format == renderer::Format::eR8G8B8A8_SRGB
			|| format == renderer::Format::eB8G8R8A8_SRGB
			|| format == renderer::Format::eA8B8G8R8_SRGB_PACK32;
	}

	GlAttachmentPoint getAttachmentPoint( GlInternal format )
	{
		if ( isDepthStencilFormat( format ) )
		{
			return GL_ATTACHMENT_POINT_DEPTH_STENCIL;
		}

		if ( isStencilFormat( format ) )
		{
			return GL_ATTACHMENT_POINT_STENCIL;
		}

		if ( isDepthFormat( format ) )
		{
			return GL_ATTACHMENT_POINT_DEPTH;
		}

		return GL_ATTACHMENT_POINT_COLOR0;
	}

	GlAttachmentPoint getAttachmentPoint( renderer::Format format )
	{
		return getAttachmentPoint( getInternal( format ) );
	}

	GlAttachmentPoint getAttachmentPoint( TextureView const & texture )
	{
		return getAttachmentPoint( getInternal( texture.getFormat() ) );
	}

	GlAttachmentType getAttachmentType( GlInternal format )
	{
		if ( isDepthStencilFormat( format ) )
		{
			return GL_ATTACHMENT_TYPE_DEPTH_STENCIL;
		}

		if ( isStencilFormat( format ) )
		{
			return GL_ATTACHMENT_TYPE_STENCIL;
		}

		if ( isDepthFormat( format ) )
		{
			return GL_ATTACHMENT_TYPE_DEPTH;
		}

		return GL_ATTACHMENT_TYPE_COLOR;
	}

	GlAttachmentType getAttachmentType( renderer::Format format )
	{
		return getAttachmentType( getInternal( format ) );
	}

	GlAttachmentType getAttachmentType( TextureView const & texture )
	{
		return getAttachmentType( getInternal( texture.getFormat() ) );
	}

	void checkCompleteness( GLenum status )
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

	FrameBuffer::FrameBuffer( Device const & device
		, RenderPass const & renderPass
		, renderer::Extent2D const & dimensions )
		: renderer::FrameBuffer{ renderPass, dimensions, renderer::FrameBufferAttachmentArray{} }
		, m_device{ device }
		, m_frameBuffer{ 0u }
		, m_renderPass{ renderPass }
	{
	}

	FrameBuffer::FrameBuffer( Device const & device
		, RenderPass const & renderPass
		, renderer::Extent2D const & dimensions
		, renderer::FrameBufferAttachmentArray && views )
		: renderer::FrameBuffer{ renderPass, dimensions, std::move( views ) }
		, m_device{ device }
		, m_renderPass{ renderPass }
	{
		auto context = m_device.getContext();
		glLogCall( context
			, glGenFramebuffers
			, 1
			, &m_frameBuffer );
		glLogCall( context
			, glBindFramebuffer
			, GL_FRAMEBUFFER
			, m_frameBuffer );

		for ( auto & attach : m_attachments )
		{
			auto & glview = static_cast< TextureView const & >( attach.getView() );
			auto & gltexture = static_cast< Texture const & >( glview.getTexture() );

			// If the image doesn't exist, it means it is a backbuffer image, hence ignore the attachment.
			if ( gltexture.hasImage() )
			{
				uint32_t index = m_renderPass.getAttachmentIndex( attach.getAttachment() );
				auto image = gltexture.getImage();
				auto mipLevel = glview.getSubResourceRange().baseMipLevel;

				FboAttachment attachment
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
					m_srgb |= isSRGBFormat( glview.getFormat() );
				}

				m_allAttaches.push_back( attachment );
				GLenum target = gltexture.getLayerCount() > 1u
					? GL_TEXTURE_2D_ARRAY
					: GL_TEXTURE_2D;

				if ( gltexture.getSamplesCount() > renderer::SampleCountFlag::e1 )
				{
					target = gltexture.getLayerCount() > 1u
						? GL_TEXTURE_2D_MULTISAMPLE_ARRAY
						: GL_TEXTURE_2D_MULTISAMPLE;
				}
				else if ( checkFlag( gltexture.getFlags(), renderer::ImageCreateFlag::eCubeCompatible ) )
				{
					target = glview.getTarget();
				}

				if ( gltexture.getLayerCount() > 1u )
				{
					glLogCall( context
						, glFramebufferTextureLayer
						, GL_FRAMEBUFFER
						, GlAttachmentPoint( attachment.point + index )
						, attachment.object
						, mipLevel
						, glview.getSubResourceRange().baseArrayLayer );
				}
				else
				{
					glLogCall( context
						, glFramebufferTexture2D
						, GL_FRAMEBUFFER
						, GlAttachmentPoint( attachment.point + index )
						, target
						, attachment.object
						, mipLevel );
				}

				checkCompleteness( context->glCheckFramebufferStatus( GL_FRAMEBUFFER ) );
			}
			else
			{
				FboAttachment attachment
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

		checkCompleteness( context->glCheckFramebufferStatus( GL_FRAMEBUFFER ) );
		glLogCall( context
			, glBindFramebuffer
			, GL_FRAMEBUFFER
			, 0 );
	}

	FrameBuffer::~FrameBuffer()
	{
		if ( m_frameBuffer > 0u )
		{
			auto context = m_device.getContext();
			glLogCall( context
				, glDeleteFramebuffers
				, 1
				, &m_frameBuffer );
		}
	}

	void FrameBuffer::setDrawBuffers( ContextLock const & context
		, AttachmentDescriptionArray const & attaches )const
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

		glLogCall( context
			, glDrawBuffers
			, GLsizei( m_drawBuffers.size() )
			, m_drawBuffers.data() );
	}

	void FrameBuffer::setDrawBuffers( ContextLock const & context
		, renderer::AttachmentReferenceArray const & attaches )const
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
				glLogCall( context
					, glDrawBuffers
					, GLsizei( m_drawBuffers.size() )
					, m_drawBuffers.data() );
			}
		}
	}
}
