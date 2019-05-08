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

	struct Attachment
	{
		Attachment( VkDevice device
			, VkImageSubresourceLayers & subresource
			, VkImage image
			, uint32_t layer )
			: object{ get( image )->getInternal() }
			, point{ getAttachmentPoint( get( image )->getFormat() ) }
			, type{ getAttachmentType( get( image )->getFormat() ) }
		{
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
				object = get( view )->getInternal();
				subresource.mipLevel = 0u;
			}
		}

		VkImageView view{ VK_NULL_HANDLE };
		GlAttachmentPoint point;
		GLuint object;
		GlAttachmentType type;
	};
	struct LayerCopy
	{
		LayerCopy( VkDevice device
			, VkImageBlit region
			, VkImage srcImage
			, VkImage dstImage
			, uint32_t layer )
			: region{ region }
			, src{ device, region.srcSubresource, srcImage, layer }
			, dst{ device, region.dstSubresource, dstImage, layer }
		{
		}

		VkImageBlit region;
		Attachment src;
		Attachment dst;
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

	void buildBlitImageCommand( VkDevice device
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

			if ( layerCopy.src.view != VK_NULL_HANDLE )
			{
				views.push_back( layerCopy.src.view );
			}

			if ( layerCopy.dst.view != VK_NULL_HANDLE )
			{
				views.push_back( layerCopy.dst.view );
			}

			// Setup source FBO
			list.push_back( makeCmd< OpType::eBindFramebuffer >( GL_FRAMEBUFFER
				, get( device )->getBlitSrcFbo() ) );
			list.push_back( makeCmd< OpType::eFramebufferTexture2D >( GL_FRAMEBUFFER
				, layerCopy.src.point
				, GL_TEXTURE_2D
				, layerCopy.src.object
				, layerCopy.region.srcSubresource.mipLevel ) );

			// Setup dst FBO
			list.push_back( makeCmd< OpType::eBindFramebuffer >( GL_FRAMEBUFFER
				, get( device )->getBlitDstFbo() ) );
			list.push_back( makeCmd< OpType::eFramebufferTexture2D >( GL_FRAMEBUFFER
				, layerCopy.dst.point
				, GL_TEXTURE_2D
				, layerCopy.dst.object
				, layerCopy.region.dstSubresource.mipLevel ) );

			// Perform the blit
			list.push_back( makeCmd< OpType::eBindFramebuffer >( GL_READ_FRAMEBUFFER
				, get( device )->getBlitSrcFbo() ) );
			list.push_back( makeCmd< OpType::eBindFramebuffer >( GL_DRAW_FRAMEBUFFER
				, get( device )->getBlitDstFbo() ) );
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
			list.push_back( makeCmd< OpType::eBindFramebuffer >( GL_READ_FRAMEBUFFER
				, 0u ) );
			list.push_back( makeCmd< OpType::eBindFramebuffer >( GL_DRAW_FRAMEBUFFER
				, 0u ) );
		}
	}
}
