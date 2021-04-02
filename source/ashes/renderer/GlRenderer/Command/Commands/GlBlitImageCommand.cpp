/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlBlitImageCommand.hpp"

#include "Command/Commands/GlCopyImageCommand.hpp"
#include "Command/Commands/GlCopyImageToBufferCommand.hpp"
#include "Command/Commands/GlCopyBufferToImageCommand.hpp"
#include "Core/GlDevice.hpp"
#include "Image/GlImage.hpp"
#include "Image/GlImageView.hpp"
#include "Miscellaneous/GlCallLogger.hpp"
#include "Miscellaneous/GlImageMemoryBinding.hpp"
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

		VkBufferImageCopy getImageToBufferCopy( VkImageBlit const & copyInfo
			, VkImage dstImage )
		{
			VkBufferImageCopy result{};
			auto & dstBinding = static_cast< ImageMemoryBinding const & >( get( dstImage )->getMemoryBinding() );
			result.bufferOffset = dstBinding.getMipLevelOffset( copyInfo.dstSubresource.mipLevel ) - dstBinding.getOffset();
			result.imageExtent.width = uint32_t( copyInfo.dstOffsets[1].x );
			result.imageExtent.height = uint32_t( copyInfo.dstOffsets[1].y );
			result.imageExtent.depth = uint32_t( copyInfo.dstOffsets[1].z );
			result.imageOffset = copyInfo.dstOffsets[0];
			result.imageSubresource = copyInfo.dstSubresource;
			return result;
		}

		VkBufferImageCopy getBufferToImageCopy( VkImageBlit const & copyInfo
			, VkImage dstImage )
		{
			VkBufferImageCopy result{};
			auto & dstBinding = static_cast< ImageMemoryBinding const & >( get( dstImage )->getMemoryBinding() );
			result.bufferOffset = dstBinding.getMipLevelOffset( copyInfo.dstSubresource.mipLevel ) - dstBinding.getOffset();
			result.imageExtent.width = uint32_t( copyInfo.dstOffsets[1].x );
			result.imageExtent.height = uint32_t( copyInfo.dstOffsets[1].y );
			result.imageExtent.depth = uint32_t( copyInfo.dstOffsets[1].z );
			result.imageOffset = copyInfo.dstOffsets[0];
			result.imageSubresource = copyInfo.dstSubresource;
			return result;
		}
	}

	void buildBlitImageCommand( ContextStateStack & stack
		, VkDevice device
		, VkImage srcImage
		, VkImage dstImage
		, VkImageBlit region
		, VkFilter filter
		, CmdList & list )
	{
		glLogCommand( list, "BlitImageCommand" );
		assert( region.srcSubresource.layerCount == region.dstSubresource.layerCount
			|| region.srcSubresource.layerCount == region.dstOffsets[1].z
			|| region.dstSubresource.layerCount == region.srcOffsets[1].z );

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
				, dstImage };

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

		if ( get( get( dstImage )->getMemoryBinding().getParent() )->getInternal() != GL_INVALID_INDEX )
		{
			buildCopyImageToBufferCommand( stack
				, device
				, getImageToBufferCopy( region, dstImage )
				, dstImage
				, get( dstImage )->getMemoryBinding()
				, list );
		}
	}
}
