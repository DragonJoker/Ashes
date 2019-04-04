/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "RenderPass/GlFrameBuffer.hpp"

#include "Command/GlQueue.hpp"
#include "Core/GlDevice.hpp"
#include "RenderPass/GlRenderPass.hpp"
#include "Image/GlImage.hpp"
#include "Image/GlImageView.hpp"

#include "ashesgl4_api.hpp"

#include <iostream>

namespace ashes::gl4
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

		bool isSRGBFormat( VkFormat format )
		{
			return format == VK_FORMAT_R8G8_SRGB
				|| format == VK_FORMAT_R8G8B8_SRGB
				|| format == VK_FORMAT_B8G8R8_SRGB
				|| format == VK_FORMAT_R8G8B8A8_SRGB
				|| format == VK_FORMAT_B8G8R8A8_SRGB
				|| format == VK_FORMAT_A8B8G8R8_SRGB_PACK32;
		}

		GlAttachmentPoint getAttachmentPoint( VkFormat format )
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

		GlAttachmentPoint getAttachmentPoint( VkImageView texture )
		{
			return getAttachmentPoint( get( texture )->getFormat() );
		}

		GlAttachmentType getAttachmentType( VkFormat format )
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

		GlAttachmentType getAttachmentType( VkImageView texture )
		{
			return getAttachmentType( get( texture )->getFormat() );
		}
	}

	void checkCompleteness( GLenum status )
	{
		switch ( status )
		{
		case 0:
			std::cerr << "An error has occured." << std::endl;
			assert( false );
			break;

		case GL_FRAMEBUFFER_STATUS_COMPLETE:
			break;

		case GL_FRAMEBUFFER_STATUS_UNDEFINED:
			std::cerr << "The specified framebuffer is the default read or draw framebuffer, but the default framebuffer does not exist." << std::endl;
			assert( false );
			break;

		case GL_FRAMEBUFFER_STATUS_INCOMPLETE_ATTACHMENT:
			std::cerr << "At least one of the framebuffer attachment points are framebuffer incomplete." << std::endl;
			assert( false );
			break;

		case GL_FRAMEBUFFER_STATUS_INCOMPLETE_MISSING_ATTACHMENT:
			std::cerr << "The framebuffer does not have at least one image attached to it." << std::endl;
			assert( false );
			break;

		case GL_FRAMEBUFFER_STATUS_INCOMPLETE_DRAW_BUFFER:
			std::cerr << "The value of GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE is GL_NONE for any color attachment point(s) named by GL_DRAW_BUFFERi." << std::endl;
			assert( false );
			break;

		case GL_FRAMEBUFFER_STATUS_INCOMPLETE_READ_BUFFER:
			std::cerr << "GL_READ_BUFFER is not GL_NONE and the value of GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE is GL_NONE for the color attachment point named by GL_READ_BUFFER." << std::endl;
			assert( false );
			break;

		case GL_FRAMEBUFFER_STATUS_UNSUPPORTED:
			std::cerr << "The combination of internal formats of the attached images violates an implementation-dependent set of restrictions." << std::endl;
			assert( false );
			break;

		case GL_FRAMEBUFFER_STATUS_INCOMPLETE_MULTISAMPLE:
			std::cerr << "One of the following:" << std::endl;
			std::cerr << "  - The value of GL_RENDERBUFFER_SAMPLES is not the same for all attached renderbuffers;" << std::endl;
			std::cerr << "  - The value of GL_TEXTURE_SAMPLES is the not same for all attached textures;" << std::endl;
			std::cerr << "  - The attached images are a mix of renderbuffers and textures, the value of GL_RENDERBUFFER_SAMPLES does not match the value of GL_TEXTURE_SAMPLES;" << std::endl;
			std::cerr << "  - The value of GL_TEXTURE_FIXED_SAMPLE_LOCATIONS is not the same for all attached textures;" << std::endl;
			std::cerr << "  - The attached images are a mix of renderbuffers and textures, the value of GL_TEXTURE_FIXED_SAMPLE_LOCATIONS is not GL_TRUE for all attached textures." << std::endl;
			assert( false );
			break;

		case GL_FRAMEBUFFER_STATUS_INCOMPLETE_LAYER_TARGETS:
			std::cerr << "At least one framebuffer attachment is layered, and any populated attachment is not layered, or all populated color attachments are not from textures of the same target." << std::endl;
			assert( false );
			break;
		}
	}

	Framebuffer::Framebuffer( VkDevice device
		, VkFramebufferCreateInfo createInfo )
		: m_device{ device }
		, m_flags{ createInfo.flags }
		, m_renderPass{ createInfo.renderPass }
		, m_attachments{ makeVector( createInfo.pAttachments, createInfo.attachmentCount ) }
		, m_width{ createInfo.width }
		, m_height{ createInfo.height }
		, m_layers{ createInfo.layers }
	{
		auto context = get( m_device )->getContext();
		glLogCall( context
			, glGenFramebuffers
			, 1
			, &m_internal );
		glLogCall( context
			, glBindFramebuffer
			, GL_FRAMEBUFFER
			, m_internal );
		uint32_t index = 0u;
		auto renderPass = get( m_renderPass );
		auto itPassAttach = renderPass->getAttachments().begin();

		for ( auto & attachment : m_attachments )
		{
			if ( ashes::isDepthOrStencilFormat( itPassAttach->format ) )
			{
				doInitialiseAttach( attachment, 0u );
			}
			else
			{
				doInitialiseAttach( attachment, index++ );
			}
		}

		checkCompleteness( context->glCheckFramebufferStatus( GL_FRAMEBUFFER ) );
		glLogCall( context
			, glBindFramebuffer
			, GL_FRAMEBUFFER
			, 0 );
	}

	Framebuffer::~Framebuffer()
	{
		if ( m_internal > 0u )
		{
			auto context = get( m_device )->getContext();
			glLogCall( context
				, glDeleteFramebuffers
				, 1
				, &m_internal );
		}
	}

	void Framebuffer::setDrawBuffers( ContextLock const & context
		, AttachmentDescriptionArray const & attaches )const
	{
		m_drawBuffers.clear();

		for ( auto & attach : attaches )
		{
			auto fboAttach = m_attachments[attach.index];
			auto fboView = get( fboAttach );
			auto fboImage = get( fboView->getImage() );

			if ( fboImage->hasInternal() )
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

	void Framebuffer::setDrawBuffers( ContextLock const & context
		, VkAttachmentReferenceArray const & attaches )const
	{
		if ( getInternal() != GL_INVALID_INDEX )
		{
			UInt32Array colours;

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

	void Framebuffer::doInitialiseAttach( VkImageView view
		, uint32_t index )
	{
		auto context = get( m_device )->getContext();
		auto image = get( view )->getImage();
		auto internal = get( view )->getInternal();
		auto mipLevel = get( view )->getSubresourceRange().baseMipLevel;

		if ( get( view )->getSubresourceRange().baseMipLevel )
		{
			if ( get( image )->getArrayLayers() == 1u )
			{
				internal = get( image )->getInternal();
			}
			else
			{
				mipLevel = 0u;
			}
		}

		Attachment attachment
		{
			getAttachmentPoint( view ),
			internal,
			getAttachmentType( view ),
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
			m_srgb |= isSRGBFormat( get( view )->getFormat() );
		}

		m_allAttaches.push_back( attachment );
		auto target = GL_TEXTURE_2D;

		if ( get( image )->getSamples() > VK_SAMPLE_COUNT_1_BIT )
		{
			target = GL_TEXTURE_2D_MULTISAMPLE;
		}

		glLogCall( context
			, glFramebufferTexture2D
			, GL_FRAMEBUFFER
			, GlAttachmentPoint( attachment.point + index )
			, target
			, attachment.object
			, mipLevel );
		checkCompleteness( context->glCheckFramebufferStatus( GL_FRAMEBUFFER ) );
	}
}
