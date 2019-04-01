/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlBlitImageCommand.hpp"

#include "Core/GlDevice.hpp"
#include "Image/GlImage.hpp"
#include "Image/GlImageView.hpp"
#include "RenderPass/GlFrameBuffer.hpp"

#include <Ashes/Image/ImageSubresourceRange.hpp>
#include <Ashes/RenderPass/FrameBufferAttachment.hpp>

namespace gl_renderer
{
	namespace
	{
		GlImageAspectFlags getMask( VkFormat format )
		{
			GlImageAspectFlags result = 0u;

			if ( ashes::isDepthFormat( format ) )
			{
				result |= GL_DEPTH_BUFFER_BIT;
			}

			if ( ashes::isStencilFormat( format ) )
			{
				result |= GL_STENCIL_BUFFER_BIT;
			}

			if ( !ashes::isDepthFormat( format )
				&& !ashes::isStencilFormat( format ) )
			{
				result |= GL_COLOR_BUFFER_BIT;
			}

			return result;
		}

		GlAttachmentPoint getAttachmentPoint( VkFormat format )
		{
			switch ( format )
			{
			case VK_FORMAT_D16_UNORM:
			case VK_FORMAT_D32_SFLOAT:
				return GL_ATTACHMENT_POINT_DEPTH;

			case VK_FORMAT_D24_UNORM_S8_UINT:
			case VK_FORMAT_D32_SFLOAT_S8_UINT:
				return GL_ATTACHMENT_POINT_DEPTH_STENCIL;

			case VK_FORMAT_S8_UINT:
				return GL_ATTACHMENT_POINT_STENCIL;

			default:
				return GL_ATTACHMENT_POINT_COLOR0;
			}
		}

		GlAttachmentType getAttachmentType( VkFormat format )
		{
			switch ( format )
			{
			case VK_FORMAT_D16_UNORM:
			case VK_FORMAT_D32_SFLOAT:
				return GL_ATTACHMENT_TYPE_DEPTH;

			case VK_FORMAT_D24_UNORM_S8_UINT:
			case VK_FORMAT_D32_SFLOAT_S8_UINT:
				return GL_ATTACHMENT_TYPE_DEPTH_STENCIL;

			case VK_FORMAT_S8_UINT:
				return GL_ATTACHMENT_TYPE_STENCIL;

			default:
				return GL_ATTACHMENT_TYPE_COLOR;
			}
		}
	}

	BlitImageCommand::Attachment::Attachment( ashes::ImageSubresourceLayers & subresource
		, Image const & image
		, uint32_t layer )
		: object{ image.getInternal() }
		, point{ getAttachmentPoint( image.getFormat() ) }
		, type{ getAttachmentType( image.getFormat() ) }
	{
		if ( image.getLayerCount() > 1u )
		{
			view = image.createView( 
			{
				ashes::ImageViewType( image.getType() ),
				image.getFormat(),
				VkComponentMapping{},
				{
					subresource.aspectMask,
					subresource.mipLevel,
					1u,
					layer,
					1u
				}
			} );
			object = static_cast< ImageView const & >( *view ).getInternal();
			subresource.mipLevel = 0u;
		}
	}

	BlitImageCommand::LayerCopy::LayerCopy( ashes::ImageBlit blitRegion
		, Image const & srcImage
		, Image const & dstImage
		, uint32_t layer )
		: region{ blitRegion }
		, src{ region.srcSubresource, srcImage, layer }
		, dst{ region.dstSubresource, dstImage, layer }
	{
	}

	BlitImageCommand::BlitImageCommand( Device const & device
		, ashes::Image const & srcImage
		, ashes::Image const & dstImage
		, std::vector< ashes::ImageBlit > const & regions
		, ashes::Filter filter )
		: CommandBase{ device }
		, m_srcTexture{ static_cast< Image const & >( srcImage ) }
		, m_dstTexture{ static_cast< Image const & >( dstImage ) }
		, m_srcFbo{ device.getBlitSrcFbo() }
		, m_dstFbo{ device.getBlitDstFbo() }
		, m_filter{ convert( filter ) }
		, m_mask{ getMask( m_srcTexture.getFormat() ) }
	{
		assert( srcImage.getLayerCount() == dstImage.getLayerCount() );

		for ( auto & region : regions )
		{
			for ( uint32_t layer = 0u; layer < srcImage.getLayerCount(); ++layer )
			{
				m_layerCopies.emplace_back( std::make_shared< BlitImageCommand::LayerCopy >( region
					, m_srcTexture
					, m_dstTexture
					, layer ) );
			}
		}
	}

	BlitImageCommand::~BlitImageCommand()
	{
	}

	void BlitImageCommand::apply( ContextLock const & context )const
	{
		auto fbo = m_device.getCurrentDepthStencilState();
		for ( auto & playerCopy : m_layerCopies )
		{
			auto & layerCopy = *playerCopy;
			// Setup source FBO
			glLogCall( context
				, glBindFramebuffer
				, GL_FRAMEBUFFER
				, m_srcFbo );
			glLogCall( context
				, glFramebufferTexture2D
				, GL_FRAMEBUFFER
				, layerCopy.src.point
				, GL_TEXTURE_2D
				, layerCopy.src.object
				, layerCopy.region.srcSubresource.mipLevel );

			// Setup dst FBO
			glLogCall( context
				, glBindFramebuffer
				, GL_FRAMEBUFFER
				, m_dstFbo );
			glLogCall( context
				, glFramebufferTexture2D
				, GL_FRAMEBUFFER
				, layerCopy.dst.point
				, GL_TEXTURE_2D
				, layerCopy.dst.object
				, layerCopy.region.dstSubresource.mipLevel );

			// Perform the blit
			glLogCall( context
				, glBindFramebuffer
				, GL_READ_FRAMEBUFFER
				, m_srcFbo );
			glLogCall( context
				, glBindFramebuffer
				, GL_DRAW_FRAMEBUFFER
				, m_dstFbo );
			glLogCall( context
				, glBlitFramebuffer
				, layerCopy.region.srcOffset.x
				, layerCopy.region.srcOffset.y
				, layerCopy.region.srcExtent.width
				, layerCopy.region.srcExtent.height
				, layerCopy.region.dstOffset.x
				, layerCopy.region.dstOffset.y
				, layerCopy.region.dstExtent.width
				, layerCopy.region.dstExtent.height
				, m_mask
				, m_filter );
			glLogCall( context
				, glBindFramebuffer
				, GL_DRAW_FRAMEBUFFER
				, m_device.getCurrentFramebuffer() );
			glLogCall( context
				, glBindFramebuffer
				, GL_READ_FRAMEBUFFER
				, 0u );
		}
	}

	CommandPtr BlitImageCommand::clone()const
	{
		return std::make_unique< BlitImageCommand >( *this );
	}
}
