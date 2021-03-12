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

	VkExtent3D operator-( VkOffset3D const & lhs, VkOffset3D const & rhs )
	{
		return VkExtent3D{ uint32_t( std::abs( lhs.x - rhs.x ) )
			, uint32_t( std::abs( lhs.y - rhs.y ) )
			, uint32_t( std::abs( lhs.z - rhs.z ) ) };
	}

	VkImageCopy convert( VkImageBlit const & src )
	{
		return VkImageCopy{ src.srcSubresource
			, src.srcOffsets[0]
			, src.dstSubresource
			, src.dstOffsets[0]
			, src.srcOffsets[1] - src.srcOffsets[0] };
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
		assert( region.srcSubresource.layerCount == region.dstSubresource.layerCount
			|| region.srcSubresource.layerCount == region.dstOffsets[1].z
			|| region.dstSubresource.layerCount == region.srcOffsets[1].z );

		if ( hasCopyImage( device )
			&& areCopyCompatible( get( srcImage )->getFormatVk(), get( dstImage )->getFormatVk() )
			&& ( region.srcOffsets[1] - region.srcOffsets[0] ) == ( region.dstOffsets[1] - region.dstOffsets[0] ) )
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
			auto layerCount = std::max( region.srcSubresource.layerCount
				, std::max( region.dstSubresource.layerCount
					, uint32_t( std::max( region.srcOffsets[1].z
						, region.dstOffsets[1].z ) ) ) );
			auto srcBaseArrayLayer = std::max( region.srcSubresource.baseArrayLayer
				, uint32_t( region.srcOffsets[0].z ) );
			auto dstBaseArrayLayer = std::max( region.dstSubresource.baseArrayLayer
				, uint32_t( region.dstOffsets[0].z ) );

			for ( uint32_t layer = 0u; layer < layerCount; ++layer )
			{
				LayerCopy layerCopy{ device
					, region
					, srcImage
					, dstImage
					, views };

				list.push_back( makeCmd< OpType::eBindSrcFramebuffer >( GL_READ_FRAMEBUFFER ) );
				layerCopy.bindSrc( stack
					, srcBaseArrayLayer + layer
					, GL_READ_FRAMEBUFFER
					, list );
				list.push_back( makeCmd< OpType::eBindDstFramebuffer >( GL_DRAW_FRAMEBUFFER ) );
				layerCopy.bindDst( stack
					, dstBaseArrayLayer + layer
					, GL_DRAW_FRAMEBUFFER
					, list );
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
			stack.applyPackAlign( list, 1 );
			list.push_back( makeCmd< OpType::eBindTexture >( dstTarget, get( dstImage )->getInternal() ) );
			list.push_back( makeCmd< OpType::eGetTexImage >( dstTarget, get( dstImage )->getGetFormat(), get( dstImage )->getGetType() ) );
			list.push_back( makeCmd< OpType::eBindTexture >( dstTarget, 0u ) );
			list.push_back( makeCmd< OpType::eBindBuffer >( GL_BUFFER_TARGET_PIXEL_PACK, 0u ) );
			list.push_back( makeCmd< OpType::eDownloadMemory >( get( dstImage )->getMemoryBinding().getParent() ) );
		}
	}
}
