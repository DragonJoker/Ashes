/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlBlitImageCommand.hpp"

#include "Core/GlDevice.hpp"
#include "Image/GlImage.hpp"
#include "Image/GlImageView.hpp"
#include "Miscellaneous/GlCallLogger.hpp"
#include "RenderPass/GlFrameBuffer.hpp"

#include "ashesgl4_api.hpp"

namespace ashes::gl4
{
	namespace
	{
		GlImageAspectFlags getMask( VkFormat format )
		{
			GlImageAspectFlags result = 0u;

			if ( isDepthFormat( format ) )
			{
				result |= GL_DEPTH_BUFFER_BIT;
			}

			if ( isStencilFormat( format ) )
			{
				result |= GL_STENCIL_BUFFER_BIT;
			}

			if ( !isDepthFormat( format )
				&& !isStencilFormat( format ) )
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

	BlitImageCommand::Attachment::Attachment( VkDevice device
		, VkImageSubresourceLayers & subresource
		, VkImage image
		, uint32_t layer )
		: object{ get( image )->getInternal() }
		, point{ getAttachmentPoint( get( image )->getFormat() ) }
		, type{ getAttachmentType( get( image )->getFormat() ) }
	{
		if ( get( image )->getArrayLayers() > 1u )
		{
			VkImageView view;
			allocate( view
				, nullptr
				, device
				, VkImageViewCreateInfo
				{
					VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
					nullptr,
					0u,
					image,
					VkImageViewType( get( image )->getType() ),
					get( image )->getFormat(),
					VkComponentMapping{},
					{
						subresource.aspectMask,
						subresource.mipLevel,
						1u,
						layer,
						1u
					},
				} );
			object = get( view )->getInternal();
			subresource.mipLevel = 0u;
		}
	}

	BlitImageCommand::LayerCopy::LayerCopy( VkDevice device
		, VkImageBlit blitRegion
		, VkImage srcImage
		, VkImage dstImage
		, uint32_t layer )
		: region{ blitRegion }
		, src{ device, region.srcSubresource, srcImage, layer }
		, dst{ device, region.dstSubresource, dstImage, layer }
	{
	}

	BlitImageCommand::BlitImageCommand( VkDevice device
		, VkImage srcImage
		, VkImage dstImage
		, VkImageBlit region
		, VkFilter filter )
		: CommandBase{ device }
		, m_srcTexture{ srcImage }
		, m_dstTexture{ dstImage }
		, m_srcFbo{ get( device )->getBlitSrcFbo() }
		, m_dstFbo{ get( device )->getBlitDstFbo() }
		, m_filter{ convert( filter ) }
		, m_mask{ getMask( get( m_srcTexture )->getFormat() ) }
	{
		assert( get( m_srcTexture )->getArrayLayers() == get( m_dstTexture )->getArrayLayers() );

		for ( uint32_t layer = 0u; layer < get( m_srcTexture )->getArrayLayers(); ++layer )
		{
			m_layerCopies.emplace_back( std::make_shared< BlitImageCommand::LayerCopy >( device
				, region
				, m_srcTexture
				, m_dstTexture
				, layer ) );
		}
	}

	BlitImageCommand::~BlitImageCommand()
	{
	}

	void BlitImageCommand::apply( ContextLock const & context )const
	{
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
				, layerCopy.region.srcOffsets[0].x
				, layerCopy.region.srcOffsets[0].y
				, layerCopy.region.srcOffsets[1].x
				, layerCopy.region.srcOffsets[1].y
				, layerCopy.region.dstOffsets[0].x
				, layerCopy.region.dstOffsets[0].y
				, layerCopy.region.dstOffsets[1].x
				, layerCopy.region.dstOffsets[1].y
				, m_mask
				, m_filter );
			glLogCall( context
				, glBindFramebuffer
				, GL_DRAW_FRAMEBUFFER
				, context->getCurrentFramebuffer() );
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
