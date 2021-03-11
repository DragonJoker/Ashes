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

	VkExtent3D convert( VkOffset3D const & src )
	{
		return VkExtent3D{ uint32_t( src.x )
			, uint32_t( src.y )
			, uint32_t( src.z ) };
	}

	VkImageCopy convert( VkImageBlit const & src )
	{
		return VkImageCopy{ src.srcSubresource
			, src.srcOffsets[0]
			, src.dstSubresource
			, src.dstOffsets[0]
			, convert( src.srcOffsets[1] ) };
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
		glLogCommand( list, "BlitImageCommand" );
		assert( region.srcSubresource.layerCount == region.dstSubresource.layerCount );

		if ( hasCopyImage( device )
			&& areCopyCompatible( get( srcImage )->getFormatVk(), get( dstImage )->getFormatVk() )
			&& region.srcOffsets[1] == region.dstOffsets[1] )
		{
			list.push_back( makeCmd< OpType::eCopyImageSubData >( get( srcImage )->getInternal()
				, convert( device
					, get( srcImage )->getType()
					, get( srcImage )->getArrayLayers()
					, get( srcImage )->getCreateFlags() )
				, get( dstImage )->getInternal()
				, convert( device
					, get( dstImage )->getType()
					, get( dstImage )->getArrayLayers()
					, get( srcImage )->getCreateFlags() )
				, convert( region ) ) );
		}
		else
		{
			auto layerCount = region.srcSubresource.layerCount;

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
				layerCopy.read( stack, list );
				list.push_back( makeCmd< OpType::eBindDstFramebuffer >( GL_DRAW_FRAMEBUFFER ) );
				layerCopy.draw( stack, list );
				list.push_back( makeCmd< OpType::eBlitFramebuffer >( layerCopy.region.srcOffsets[0].x
					, layerCopy.region.srcOffsets[0].y
					, layerCopy.region.srcOffsets[1].x
					, layerCopy.region.srcOffsets[1].y
					, layerCopy.region.dstOffsets[0].x
					, layerCopy.region.dstOffsets[0].y
					, layerCopy.region.dstOffsets[1].x
					, layerCopy.region.dstOffsets[1].y
					, getMask( get( srcImage )->getFormatVk() )
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

		if ( get( get( dstImage )->getMemoryBinding().getParent() )->getInternal() != GL_INVALID_INDEX )
		{
			auto dstTarget = convert( device
				, get( dstImage )->getType()
				, get( dstImage )->getArrayLayers()
				, get( dstImage )->getCreateFlags() );
			list.push_back( makeCmd< OpType::eBindBuffer >( GL_BUFFER_TARGET_PIXEL_PACK, get( get( dstImage )->getMemoryBinding().getParent() )->getInternal() ) );
			list.push_back( makeCmd< OpType::eBindTexture >( dstTarget, get( dstImage )->getInternal() ) );
			list.push_back( makeCmd< OpType::eGetTexImage >( dstTarget, get( dstImage )->getFormatFormat(), get( dstImage )->getFormatType() ) );
			list.push_back( makeCmd< OpType::eBindTexture >( dstTarget, 0u ) );
			list.push_back( makeCmd< OpType::eBindBuffer >( GL_BUFFER_TARGET_PIXEL_PACK, 0u ) );
			list.push_back( makeCmd< OpType::eDownloadMemory >( get( dstImage )->getMemoryBinding().getParent() ) );
		}
	}
}
