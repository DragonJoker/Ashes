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

#include "ashesgl_api.hpp"

#include <iostream>

namespace ashes::gl
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

	namespace gl3
	{
		FboAttachment initialiseAttachment( uint32_t referenceIndex
			, VkImageView view
			, uint32_t index
			, bool & multisampled )
		{
			auto image = get( view )->getImage();
			multisampled = get( image )->getSamples() > VK_SAMPLE_COUNT_1_BIT;

			auto layersCount = get( image )->getArrayLayers();
			bool isCube = checkFlag( get( image )->getCreateFlags()
				, VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT );

			FboAttachment attachment{};
			attachment.referenceIndex = referenceIndex;
			attachment.point = getAttachmentPoint( view );
			attachment.type = getAttachmentType( view );
			attachment.originalObject = get( image )->getInternal();
			attachment.originalMipLevel = get( view )->getSubresourceRange().baseMipLevel;
			attachment.object = get( view )->getInternal();
			attachment.mipLevel = attachment.originalMipLevel;
			attachment.imgLayerCount = std::max( get( image )->getArrayLayers(), get( image )->getDimensions().depth );
			attachment.viewLayerCount = ( ( get( view )->getType() == VK_IMAGE_VIEW_TYPE_3D )
				? attachment.imgLayerCount
				: get( view )->getSubresourceRange().layerCount );
			attachment.baseArrayLayer = ( ( ( !isCube && attachment.imgLayerCount > 1u ) || ( isCube && attachment.imgLayerCount > 6u ) )
				? get( view )->getSubresourceRange().baseArrayLayer
				: 0u );
			attachment.target = ( get( image )->getType() == VK_IMAGE_TYPE_3D
				? GL_TEXTURE_3D
				: ( multisampled
					? ( attachment.viewLayerCount > 1u
						? GL_TEXTURE_2D_MULTISAMPLE_ARRAY
						: GL_TEXTURE_2D_MULTISAMPLE )
					: ( isCube
						? get( view )->getTextureType()
						: ( attachment.viewLayerCount > 1u
							? GL_TEXTURE_2D_ARRAY
							: GL_TEXTURE_2D ) ) ) );
			attachment.index = index;
			attachment.isSrgb = isSRGBFormat( get( image )->getFormatVk() );
			return attachment;
		}

		FboAttachment initialiseAttachment( VkDevice device
			, VkImageSubresourceLayers & subresource
			, VkImage image
			, VkImageView & view )
		{
			FboAttachment result{ 0u
				, getAttachmentPoint( get( image )->getFormatVk() )
				, get( image )->getInternal()
				, getAttachmentType( get( image )->getFormatVk() )
				, ( ( get( image )->getType() == VK_IMAGE_TYPE_3D )
					? GL_TEXTURE_3D
					: ( ( get( image )->getType() == VK_IMAGE_TYPE_2D )
						? ( get( image )->getSamples() > VK_SAMPLE_COUNT_1_BIT
							? GL_TEXTURE_2D_MULTISAMPLE
							: ( checkFlag( get( image )->getCreateFlags(), VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT )
								? GL_TEXTURE_CUBE_POSITIVE_X
								: GL_TEXTURE_2D ) )
						: GL_TEXTURE_1D ) )
				, subresource.mipLevel
				, 0u };
			result.isSrgb = isSRGBFormat( get( image )->getFormatVk() );

			if ( get( image )->getArrayLayers() > 1u )
			{
				result.mipLevel = 0u;
			}

			return result;
		}
	}

	namespace gl4
	{
		FboAttachment initialiseAttachment( uint32_t referenceIndex
			, VkImageView view
			, uint32_t index
			, bool & multisampled )
		{
			auto image = get( view )->getImage();
			multisampled = get( image )->getSamples() > VK_SAMPLE_COUNT_1_BIT;

			FboAttachment attachment{};
			attachment.referenceIndex = referenceIndex;
			attachment.point = getAttachmentPoint( view );
			attachment.type = getAttachmentType( view );
			attachment.originalObject = get( image )->getInternal();
			attachment.originalMipLevel = get( view )->getSubresourceRange().baseMipLevel;
			attachment.object = get( view )->getInternal();
			attachment.mipLevel = attachment.originalMipLevel;
			attachment.imgLayerCount = std::max( get( image )->getArrayLayers(), get( image )->getDimensions().depth );
			attachment.viewLayerCount = ( ( get( view )->getType() == VK_IMAGE_VIEW_TYPE_3D )
				? attachment.imgLayerCount
				: get( view )->getSubresourceRange().layerCount );
			attachment.target = ( get( image )->getType() == VK_IMAGE_TYPE_3D
				? GL_TEXTURE_3D
				: ( get( image )->getType() == VK_IMAGE_TYPE_2D
					? ( attachment.viewLayerCount > 1u
						? ( multisampled
							? GL_TEXTURE_2D_MULTISAMPLE_ARRAY
							: GL_TEXTURE_2D_ARRAY )
						: ( multisampled
							? GL_TEXTURE_2D_MULTISAMPLE
							: GL_TEXTURE_2D ) )
					: ( attachment.viewLayerCount > 1u
						? GL_TEXTURE_1D_ARRAY
						: GL_TEXTURE_1D ) ) );
			attachment.index = index;
			attachment.isSrgb = isSRGBFormat( get( image )->getFormatVk() );

			if ( get( view )->getSubresourceRange().baseMipLevel )
			{
				if ( get( image )->getArrayLayers() == 1u )
				{
					attachment.object = get( image )->getInternal();
				}
				else
				{
					attachment.mipLevel = 0u;
				}
			}

			return attachment;
		}

		FboAttachment initialiseAttachment( VkDevice device
			, VkImageSubresourceLayers & subresource
			, VkImage image
			, VkImageView & view )
		{
			FboAttachment result{ 0u
				, getAttachmentPoint( subresource.aspectMask )
				, get( image )->getInternal()
				, getAttachmentType( subresource.aspectMask )
				, ( ( get( image )->getType() == VK_IMAGE_TYPE_3D )
					? GL_TEXTURE_3D
					: ( ( get( image )->getType() == VK_IMAGE_TYPE_2D )
						? ( get( image )->getSamples() > VK_SAMPLE_COUNT_1_BIT
							? GL_TEXTURE_2D_MULTISAMPLE
							: GL_TEXTURE_2D )
						: GL_TEXTURE_1D ) )
				, subresource.mipLevel
				, 0u };
			result.isSrgb = isSRGBFormat( get( image )->getFormatVk() );

			if ( get( image )->getArrayLayers() > 1u )
			{
				result.mipLevel = 0u;
			}

			return result;
		}
	}

	FboAttachment initialiseAttachment( VkDevice device
		, uint32_t referenceIndex
		, VkImageView view
		, uint32_t index
		, bool & multisampled )
	{
		if ( hasTextureViews( device ) )
		{
			return gl4::initialiseAttachment( referenceIndex
				, view
				, index
				, multisampled );
		}

		return gl3::initialiseAttachment( referenceIndex
			, view
			, index
			, multisampled );
	}

	FboAttachment initialiseAttachment( VkDevice device
		, VkImageSubresourceLayers & subresource
		, VkImage image
		, VkImageView & view )
	{
		if ( hasTextureViews( device ) )
		{
			return gl4::initialiseAttachment( device
				, subresource
				, image
				, view );
		}

		return gl3::initialiseAttachment( device
			, subresource
			, image
			, view );
	}

	bool isSRGBFormat( VkFormat format )
	{
		return format == VK_FORMAT_R8_SRGB
			|| format == VK_FORMAT_R8G8_SRGB
			|| format == VK_FORMAT_R8G8B8_SRGB
			|| format == VK_FORMAT_B8G8R8_SRGB
			|| format == VK_FORMAT_R8G8B8A8_SRGB
			|| format == VK_FORMAT_B8G8R8A8_SRGB
			|| format == VK_FORMAT_A8B8G8R8_SRGB_PACK32
			|| format == VK_FORMAT_BC1_RGB_SRGB_BLOCK
			|| format == VK_FORMAT_BC1_RGBA_SRGB_BLOCK
			|| format == VK_FORMAT_BC2_SRGB_BLOCK
			|| format == VK_FORMAT_BC3_SRGB_BLOCK
			|| format == VK_FORMAT_BC7_SRGB_BLOCK
			|| format == VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK
			|| format == VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK
			|| format == VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK
			|| format == VK_FORMAT_ASTC_4x4_SRGB_BLOCK
			|| format == VK_FORMAT_ASTC_5x4_SRGB_BLOCK
			|| format == VK_FORMAT_ASTC_5x5_SRGB_BLOCK
			|| format == VK_FORMAT_ASTC_6x5_SRGB_BLOCK
			|| format == VK_FORMAT_ASTC_6x6_SRGB_BLOCK
			|| format == VK_FORMAT_ASTC_8x5_SRGB_BLOCK
			|| format == VK_FORMAT_ASTC_8x6_SRGB_BLOCK
			|| format == VK_FORMAT_ASTC_8x8_SRGB_BLOCK
			|| format == VK_FORMAT_ASTC_10x5_SRGB_BLOCK
			|| format == VK_FORMAT_ASTC_10x6_SRGB_BLOCK
			|| format == VK_FORMAT_ASTC_10x8_SRGB_BLOCK
			|| format == VK_FORMAT_ASTC_10x10_SRGB_BLOCK
			|| format == VK_FORMAT_ASTC_12x10_SRGB_BLOCK
			|| format == VK_FORMAT_ASTC_12x12_SRGB_BLOCK;
	}

	GlAttachmentPoint getAttachmentPoint( VkImageAspectFlags aspectMask )
	{
		if ( checkFlag( aspectMask, VkImageAspectFlagBits::VK_IMAGE_ASPECT_DEPTH_BIT )
			&& checkFlag( aspectMask, VkImageAspectFlagBits::VK_IMAGE_ASPECT_STENCIL_BIT ) )
		{
			return GL_ATTACHMENT_POINT_DEPTH_STENCIL;
		}

		if ( checkFlag( aspectMask, VkImageAspectFlagBits::VK_IMAGE_ASPECT_STENCIL_BIT ) )
		{
			return GL_ATTACHMENT_POINT_STENCIL;
		}

		if ( checkFlag( aspectMask, VkImageAspectFlagBits::VK_IMAGE_ASPECT_DEPTH_BIT ) )
		{
			return GL_ATTACHMENT_POINT_DEPTH;
		}

		return GL_ATTACHMENT_POINT_COLOR0;
	}

	GlAttachmentPoint getAttachmentPoint( VkImageView texture )
	{
		return getAttachmentPoint( get( texture )->getSubresourceRange().aspectMask );
	}

	GlAttachmentPoint getAttachmentPoint( VkFormat format )
	{
		if ( isDepthStencilFormat( format ) )
		{
			return GL_ATTACHMENT_POINT_DEPTH_STENCIL;
		}

		if ( isDepthFormat( format ) )
		{
			return GL_ATTACHMENT_POINT_DEPTH;
		}

		if ( isStencilFormat( format ) )
		{
			return GL_ATTACHMENT_POINT_STENCIL;
		}

		return GL_ATTACHMENT_POINT_COLOR0;
	}

	GlAttachmentType getAttachmentType( VkImageAspectFlags aspectMask )
	{
		if ( checkFlag( aspectMask, VkImageAspectFlagBits::VK_IMAGE_ASPECT_DEPTH_BIT )
			&& checkFlag( aspectMask, VkImageAspectFlagBits::VK_IMAGE_ASPECT_STENCIL_BIT ) )
		{
			return GL_ATTACHMENT_TYPE_DEPTH_STENCIL;
		}

		if ( checkFlag( aspectMask, VkImageAspectFlagBits::VK_IMAGE_ASPECT_STENCIL_BIT ) )
		{
			return GL_ATTACHMENT_TYPE_STENCIL;
		}

		if ( checkFlag( aspectMask, VkImageAspectFlagBits::VK_IMAGE_ASPECT_DEPTH_BIT ) )
		{
			return GL_ATTACHMENT_TYPE_DEPTH;
		}

		return GL_ATTACHMENT_TYPE_COLOR;
	}

	GlAttachmentType getAttachmentType( VkImageView texture )
	{
		return getAttachmentType( get( texture )->getSubresourceRange().aspectMask );
	}

	GlAttachmentType getAttachmentType( VkFormat format )
	{
		if ( isDepthStencilFormat( format ) )
		{
			return GL_ATTACHMENT_TYPE_DEPTH_STENCIL;
		}

		if ( isDepthFormat( format ) )
		{
			return GL_ATTACHMENT_TYPE_DEPTH;
		}

		if ( isStencilFormat( format ) )
		{
			return GL_ATTACHMENT_TYPE_STENCIL;
		}

		return GL_ATTACHMENT_TYPE_COLOR;
	}

	template< typename VkObjectT >
	void doCheckCompleteness( VkObjectT object
		, GLenum status
		, void ( *report )( VkObjectT
			, VkResult
			, std::string const &
			, std::string const & ) )
	{
		switch ( status )
		{
		case 0:
			report( object, VK_ERROR_INITIALIZATION_FAILED, "Framebuffer Completeness"
				, "An error has occured." );
			break;

		case GL_FRAMEBUFFER_STATUS_COMPLETE:
			break;

		case GL_FRAMEBUFFER_STATUS_UNDEFINED:
			report( object, VK_ERROR_INITIALIZATION_FAILED, "Framebuffer Completeness"
				, "The specified framebuffer is the default read or draw framebuffer, but the default framebuffer does not exist." );
			break;

		case GL_FRAMEBUFFER_STATUS_INCOMPLETE_ATTACHMENT:
			report( object, VK_ERROR_INITIALIZATION_FAILED, "Framebuffer Completeness"
				, "At least one of the framebuffer attachment points are framebuffer incomplete." );
			break;

		case GL_FRAMEBUFFER_STATUS_INCOMPLETE_MISSING_ATTACHMENT:
			report( object, VK_ERROR_INITIALIZATION_FAILED, "Framebuffer Completeness"
				, "The framebuffer does not have at least one image attached to it." );
			break;

		case GL_FRAMEBUFFER_STATUS_INCOMPLETE_DRAW_BUFFER:
			report( object, VK_ERROR_INITIALIZATION_FAILED, "Framebuffer Completeness"
				, "The value of GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE is GL_NONE for any color attachment point(s) named by GL_DRAW_BUFFERi." );
			break;

		case GL_FRAMEBUFFER_STATUS_INCOMPLETE_READ_BUFFER:
			report( object, VK_ERROR_INITIALIZATION_FAILED, "Framebuffer Completeness"
				, "GL_READ_BUFFER is not GL_NONE and the value of GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE is GL_NONE for the color attachment point named by GL_READ_BUFFER." );
			break;

		case GL_FRAMEBUFFER_STATUS_UNSUPPORTED:
			report( object, VK_ERROR_INITIALIZATION_FAILED, "Framebuffer Completeness"
				, "The combination of internal formats of the attached images violates an implementation-dependent set of restrictions." );
			break;

		case GL_FRAMEBUFFER_STATUS_INCOMPLETE_MULTISAMPLE:
			{
				std::stringstream stream;
				stream << "One of the following:" << std::endl;
				stream << "  - The value of GL_RENDERBUFFER_SAMPLES is not the same for all attached renderbuffers;" << std::endl;
				stream << "  - The value of GL_TEXTURE_SAMPLES is the not same for all attached textures;" << std::endl;
				stream << "  - The attached images are a mix of renderbuffers and textures, the value of GL_RENDERBUFFER_SAMPLES does not match the value of GL_TEXTURE_SAMPLES;" << std::endl;
				stream << "  - The value of GL_TEXTURE_FIXED_SAMPLE_LOCATIONS is not the same for all attached textures;" << std::endl;
				stream << "  - The attached images are a mix of renderbuffers and textures, the value of GL_TEXTURE_FIXED_SAMPLE_LOCATIONS is not GL_TRUE for all attached textures." << std::endl;
				report( object, VK_ERROR_INITIALIZATION_FAILED, "Framebuffer Completeness"
					, stream.str() );
			}
			break;

		case GL_FRAMEBUFFER_STATUS_INCOMPLETE_LAYER_TARGETS:
			report( object, VK_ERROR_INITIALIZATION_FAILED, "Framebuffer Completeness"
				, "At least one framebuffer attachment is layered, and any populated attachment is not layered, or all populated color attachments are not from textures of the same target." );
			break;
		}
	}

	void checkCompleteness( VkDevice device
		, GLenum status )
	{
		doCheckCompleteness( device, status, reportError< VkDevice > );
	}

	void checkCompleteness( VkSwapchainKHR swapchain
		, GLenum status )
	{
		doCheckCompleteness( swapchain, status, reportWarning< VkSwapchainKHR > );
	}

	void checkCompleteness( VkFramebuffer framebuffer
		, GLenum status )
	{
		doCheckCompleteness( framebuffer, status, reportWarning< VkFramebuffer > );
	}

	Framebuffer::Framebuffer( VkAllocationCallbacks const * allocInfo
		, VkDevice device
		, VkFramebufferCreateInfo createInfo )
		: m_device{ device }
		, m_flags{ createInfo.flags }
		, m_renderPass{ createInfo.renderPass }
		, m_attachments{ makeVector( createInfo.pAttachments, createInfo.attachmentCount ) }
		, m_dimensions{ createInfo.width, createInfo.height }
		, m_layers{ createInfo.layers }
	{
		doInitialiseAttaches();

		if ( !isEmpty() )
		{
			doCreateFramebuffer();
		}
	}

	Framebuffer::Framebuffer( VkAllocationCallbacks const * allocInfo
		, VkDevice device
		, GLuint name )
		: m_device{ device }
	{
		m_internal = name;
	}

	Framebuffer::~Framebuffer()
	{
		if ( m_internal != GL_INVALID_INDEX )
		{
			auto context = get( m_device )->getContext();
			glLogCall( context
				, glDeleteFramebuffers
				, 1
				, &m_internal );
		}
	}

	std::vector< GlAttachmentPoint > Framebuffer::getDrawBuffers( ArrayView< VkAttachmentReference const > const & references )const
	{
		assert( getInternal() != GL_INVALID_INDEX );
		std::vector< GlAttachmentPoint > drawBuffers;

		if ( !isEmpty() )
		{
			uint32_t index = 0u;
			auto & attaches = getRenderableAttaches();

			for ( auto & reference : references )
			{
				if ( reference.attachment != VK_ATTACHMENT_UNUSED )
				{
					auto attachIt = std::find_if( attaches.begin()
						, attaches.end()
						, [&reference]( FboAttachment const & lookup )
						{
							return lookup.referenceIndex == reference.attachment;
						} );
					assert( attachIt != attaches.end() );
					auto & attach = *attachIt;

					if ( attach.point != GL_ATTACHMENT_POINT_DEPTH
						&& attach.point != GL_ATTACHMENT_POINT_STENCIL
						&& attach.point != GL_ATTACHMENT_POINT_DEPTH_STENCIL )
					{
						drawBuffers.push_back( GlAttachmentPoint( attach.point + index ) );
					}

					++index;
				}
			}

			if ( m_drawBuffers != drawBuffers )
			{
				m_drawBuffers = drawBuffers;
			}
		}

		return drawBuffers;
	}

	FboAttachment Framebuffer::getAttachment( VkAttachmentReference const & reference )const
	{
		if ( reference.attachment != VK_ATTACHMENT_UNUSED )
		{
			auto & attachments = getAttachments();
			auto fboAttach = attachments[reference.attachment];
			auto fboView = get( fboAttach );

			auto & attaches = getRenderableAttaches();
			auto attachIt = std::find_if( attaches.begin()
				, attaches.end()
				, [&reference]( FboAttachment const & lookup )
				{
					return lookup.referenceIndex == reference.attachment;
				} );
			assert( attachIt != attaches.end() );
			return *attachIt;
		}

		return FboAttachment{};
	}

	bool Framebuffer::hasOnlySwapchainImage()const
	{
		return m_attachments.end() == std::find_if( m_attachments.begin()
			, m_attachments.end()
			, []( VkImageView view )
			{
				return !get( get( view )->getImage() )->isSwapchainImage();
			} );
	}

	bool Framebuffer::hasSwapchainImage()const
	{
		return m_attachments.end() != std::find_if( m_attachments.begin()
			, m_attachments.end()
			, []( VkImageView view )
			{
				return get( get( view )->getImage() )->isSwapchainImage();
			} );
	}

	void Framebuffer::doInitialiseAttaches()
	{
		auto renderPass = get( m_renderPass );
		uint32_t index = 0u;
		uint32_t msIndex = 0u;

		for ( auto & passAttach : renderPass->getFboAttachable() )
		{
			if ( passAttach.attachment != VK_ATTACHMENT_UNUSED )
			{
				bool multisampled{ false };
				assert( passAttach.attachment < m_attachments.size() );
				auto view = m_attachments[passAttach.attachment];
				auto attachment = initialiseAttachment( m_device
					, passAttach.attachment
					, view
					, ( ashes::isDepthOrStencilFormat( get( view )->getFormatVk() )
						? 0u
						: ( get( get( view )->getImage() )->getSamples() > VK_SAMPLE_COUNT_1_BIT
							? msIndex++
							: index++ ) )
					, multisampled );
				auto attach = renderPass->getAttachment( passAttach );
				doInitialiseAttach( attachment
					, multisampled
					, isSRGBFormat( get( view )->getFormatVk() ) );
				m_renderableAttaches.push_back( attachment );
			}
		}

		for ( auto & passAttach : renderPass->getResolveAttachments() )
		{
			if ( passAttach.attachment != VK_ATTACHMENT_UNUSED )
			{
				bool multisampled{ false };
				assert( passAttach.attachment < m_attachments.size() );
				auto view = m_attachments[passAttach.attachment];
				auto attachment = initialiseAttachment( m_device
					, passAttach.attachment
					, view
					, ( ashes::isDepthOrStencilFormat( get( view )->getFormatVk() )
						? 0u
						: ( get( get( view )->getImage() )->getSamples() > VK_SAMPLE_COUNT_1_BIT
							? msIndex++
							: index++ ) )
					, multisampled );
				m_resolveAttaches.push_back( attachment );
			}
		}
	}

	void Framebuffer::doCreateFramebuffer()
	{
		auto context = get( m_device )->getContext();
		glLogCreateCall( context
			, glGenFramebuffers
			, 1
			, &m_internal );
	}

	void Framebuffer::doInitialiseAttach( FboAttachment attachment
		, bool multisampled
		, bool isSRGB )
	{
		m_multisampled = m_multisampled || multisampled;

		if ( attachment.isDepthOrStencil() )
		{
			assert( attachment.index == 0u );

			if ( multisampled )
			{
				m_depthStencilMsAttach = attachment;
			}
			else
			{
				m_depthStencilAttach = attachment;
			}
		}
		else
		{
			if ( multisampled )
			{
				m_colourMsAttaches.push_back( attachment );
			}
			else
			{
				m_colourAttaches.push_back( attachment );
			}

			m_allColourAttaches.push_back( attachment );
			m_srgb |= isSRGB;
		}
	}
}
