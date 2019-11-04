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
	}

	FboAttachment makeFboAttachment( VkDevice device
		, VkImageSubresourceLayers & subresource
		, VkImage image
		, uint32_t layer
		, VkImageView & view )
	{
		FboAttachment result
		{
			getAttachmentPoint( get( image )->getFormat() ),
			get( image )->getInternal(),
			getAttachmentType( get( image )->getFormat() ),
			get( image )->getSamples() > VK_SAMPLE_COUNT_1_BIT ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D,
			subresource.mipLevel,
			0u,
		};

		if ( get( image )->getArrayLayers() > 1u )
		{
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
			result.object = get( view )->getInternal();
			result.mipLevel = 0u;
		}

		return result;
	}

	struct LayerCopy
	{
		LayerCopy( VkDevice device
			, VkImageBlit region
			, VkImage srcImage
			, VkImage dstImage
			, uint32_t layer )
			: region{ region }
			, src{ makeFboAttachment( device, region.srcSubresource, srcImage, layer, srcView ) }
			, dst{ makeFboAttachment( device, region.dstSubresource, dstImage, layer, dstView ) }
		{
		}

		VkImageBlit region;
		VkImageView srcView{ VK_NULL_HANDLE };
		VkImageView dstView{ VK_NULL_HANDLE };
		FboAttachment src;
		FboAttachment dst;
	};

	void apply( ContextLock const & context
		, CmdBlitFramebuffer const & cmd )
	{
		glLogCall( context
			, glBlitFramebuffer
			, cmd.srcL
			, cmd.srcT
			, cmd.srcR
			, cmd.srcB
			, cmd.dstL
			, cmd.dstT
			, cmd.dstR
			, cmd.dstB
			, cmd.mask
			, cmd.filter );
	}

	void buildBlitImageCommand( ContextStateStack & stack
		, VkDevice device
		, VkImage srcImage
		, VkImage dstImage
		, VkImageBlit region
		, VkFilter filter
		, CmdList & list
		, VkImageViewArray & views )
	{
		assert( get( srcImage )->getArrayLayers() == get( dstImage )->getArrayLayers() );

		for ( uint32_t layer = 0u; layer < get( srcImage )->getArrayLayers(); ++layer )
		{
			LayerCopy layerCopy
			{
				device,
				region,
				srcImage,
				dstImage,
				layer
			};

			if ( layerCopy.srcView != VK_NULL_HANDLE )
			{
				views.push_back( layerCopy.srcView );
			}

			if ( layerCopy.dstView != VK_NULL_HANDLE )
			{
				views.push_back( layerCopy.dstView );
			}

			// Setup source FBO
			list.push_back( makeCmd< OpType::eInitFramebuffer >( &get( get( device )->getBlitSrcFbo() )->getInternal() ) );
			list.push_back( makeCmd< OpType::eBindFramebuffer >( GL_FRAMEBUFFER
				, get( device )->getBlitSrcFbo() ) );
			list.push_back( makeCmd< OpType::eFramebufferTexture2D >( GL_FRAMEBUFFER
				, layerCopy.src.point
				, layerCopy.src.target
				, layerCopy.src.object
				, layerCopy.region.srcSubresource.mipLevel ) );
			list.push_back( makeCmd< OpType::eBindFramebuffer >( GL_FRAMEBUFFER
				, nullptr ) );

			// Setup dst FBO
			list.push_back( makeCmd< OpType::eInitFramebuffer >( &get( get( device )->getBlitDstFbo() )->getInternal() ) );
			list.push_back( makeCmd< OpType::eBindFramebuffer >( GL_FRAMEBUFFER
				, get( device )->getBlitDstFbo() ) );
			list.push_back( makeCmd< OpType::eFramebufferTexture2D >( GL_FRAMEBUFFER
				, layerCopy.dst.point
				, layerCopy.dst.target
				, layerCopy.dst.object
				, layerCopy.region.dstSubresource.mipLevel ) );
			list.push_back( makeCmd< OpType::eBindFramebuffer >( GL_FRAMEBUFFER
				, nullptr ) );

			// Perform the blit
			list.push_back( makeCmd< OpType::eBindFramebuffer >( GL_READ_FRAMEBUFFER
				, get( device )->getBlitSrcFbo() ) );
			list.push_back( makeCmd< OpType::eReadBuffer >( uint32_t( layerCopy.src.point ) ) );
			list.push_back( makeCmd< OpType::eBindFramebuffer >( GL_DRAW_FRAMEBUFFER
				, get( device )->getBlitDstFbo() ) );
			list.push_back( makeCmd< OpType::eDrawBuffer >( uint32_t( layerCopy.dst.point ) ) );
			list.push_back( makeCmd< OpType::eBlitFramebuffer >( layerCopy.region.srcOffsets[0].x
				, layerCopy.region.srcOffsets[0].y
				, layerCopy.region.srcOffsets[1].x
				, layerCopy.region.srcOffsets[1].y
				, layerCopy.region.dstOffsets[0].x
				, layerCopy.region.dstOffsets[0].y
				, layerCopy.region.dstOffsets[1].x
				, layerCopy.region.dstOffsets[1].y
				, getMask( get( srcImage )->getFormat() )
				, convert( filter ) ) );

			// Unbind
			list.push_back( makeCmd< OpType::eBindFramebuffer >( GL_READ_FRAMEBUFFER
				, nullptr ) );
			list.push_back( makeCmd< OpType::eBindFramebuffer >( GL_DRAW_FRAMEBUFFER
				, nullptr ) );

			if ( stack.hasCurrentFramebuffer() )
			{
				stack.setCurrentFramebuffer( VK_NULL_HANDLE );
			}
		}
	}
}
