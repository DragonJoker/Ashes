/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlBlitImageCommand.hpp"

#include "Command/Commands/GlCopyImageCommand.hpp"
#include "Core/GlDevice.hpp"
#include "Image/GlImage.hpp"
#include "Image/GlImageView.hpp"
#include "Miscellaneous/GlCallLogger.hpp"
#include "RenderPass/GlFrameBuffer.hpp"

#include "ashesgl_api.hpp"

namespace ashes::gl
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

	struct LayerCopy
	{
		LayerCopy( VkDevice device
			, VkImageBlit origRegion
			, VkImage srcImage
			, VkImage dstImage
			, uint32_t layer
			, VkImageViewArray & views )
			: region{ origRegion }
			, src{ initialiseAttachment( device, origRegion.srcSubresource, srcImage, layer, srcView ) }
			, dst{ initialiseAttachment( device, origRegion.dstSubresource, dstImage, layer, dstView ) }
		{
			if ( srcView != VK_NULL_HANDLE )
			{
				views.push_back( srcView );
				region.srcSubresource.mipLevel = 0u;
			}

			if ( dstView != VK_NULL_HANDLE )
			{
				views.push_back( dstView );
				region.dstSubresource.mipLevel = 0u;
			}
		}

		void bindSrc( uint32_t layer
			, CmdList & list )
		{
			src.bind( region.srcSubresource, layer, list );
		}

		void bindDst( uint32_t layer
			, CmdList & list )
		{
			dst.bind( region.dstSubresource, layer, list );
		}

		GlAttachmentPoint getSrcPoint()const
		{
			return src.point;
		}

		GlAttachmentPoint getDstPoint()const
		{
			return dst.point;
		}

		bool isSrcDepthOrStencil()const
		{
			return src.isDepthOrStencil();
		}

		bool isDstDepthOrStencil()const
		{
			return dst.isDepthOrStencil();
		}

		VkImageBlit region;

	private:
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
		assert( get( srcImage )->getArrayLayers() == get( dstImage )->getArrayLayers()
			&& get( srcImage )->getDimensions().depth == get( dstImage )->getDimensions().depth );
		auto layerCount = get( srcImage )->getArrayLayers();

		for ( uint32_t layer = 0u; layer < layerCount; ++layer )
		{
			LayerCopy layerCopy{ device
				, region
				, srcImage
				, dstImage
				, layer
				, views };

			// Setup source FBO
			list.push_back( makeCmd< OpType::eBindSrcFramebuffer >( GL_FRAMEBUFFER ) );
			layerCopy.bindSrc( layer, list );
			list.push_back( makeCmd< OpType::eBindFramebuffer >( GL_FRAMEBUFFER
				, nullptr ) );

			// Setup dst FBO
			list.push_back( makeCmd< OpType::eBindDstFramebuffer >( GL_FRAMEBUFFER ) );
			layerCopy.bindDst( layer, list );
			list.push_back( makeCmd< OpType::eBindFramebuffer >( GL_FRAMEBUFFER
				, nullptr ) );

			// Perform the blit
			list.push_back( makeCmd< OpType::eBindSrcFramebuffer >( GL_READ_FRAMEBUFFER ) );

			if ( !layerCopy.isSrcDepthOrStencil() )
			{
				list.push_back( makeCmd< OpType::eReadBuffer >( layerCopy.getSrcPoint() ) );
			}

			list.push_back( makeCmd< OpType::eBindDstFramebuffer >( GL_DRAW_FRAMEBUFFER ) );

			if ( !layerCopy.isDstDepthOrStencil() )
			{
				list.push_back( makeCmd< OpType::eDrawBuffers >( layerCopy.getDstPoint() ) );
			}

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

		if ( get( get( dstImage )->getMemoryBinding().getParent() )->getInternal() != GL_INVALID_INDEX )
		{
			auto dstTarget = convert( device
				, get( dstImage )->getType()
				, get( dstImage )->getArrayLayers()
				, get( dstImage )->getCreateFlags() );
			list.push_back( makeCmd< OpType::eBindBuffer >( GL_BUFFER_TARGET_PIXEL_PACK, get( get( dstImage )->getMemoryBinding().getParent() )->getInternal() ) );
			list.push_back( makeCmd< OpType::eBindTexture >( dstTarget, get( dstImage )->getInternal() ) );
			auto internal = getInternalFormat( get( dstImage )->getFormat() );
			list.push_back( makeCmd< OpType::eGetTexImage >( dstTarget, getFormat( internal ), getType( internal ) ) );
			list.push_back( makeCmd< OpType::eBindTexture >( dstTarget, 0u ) );
			list.push_back( makeCmd< OpType::eBindBuffer >( GL_BUFFER_TARGET_PIXEL_PACK, 0u ) );
			list.push_back( makeCmd< OpType::eDownloadMemory >( get( dstImage )->getMemoryBinding().getParent() ) );
		}
	}
}
