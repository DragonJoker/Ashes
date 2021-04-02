/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlCopyImageCommand.hpp"
#include "Command/Commands/GlCopyImageToBufferCommand.hpp"
#include "Miscellaneous/GlImageMemoryBinding.hpp"

#include "Image/GlImage.hpp"
#include "Image/GlImageView.hpp"

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

		ByteArray allocateData( VkImage src )
		{
			auto dimensions = get( src )->getDimensions();
			auto texels = dimensions.width * dimensions.height * dimensions.depth;
			return ByteArray( texels * ashes::getSize( get( src )->getFormatVk() ), uint8_t( 0 ) );
		}

		ByteArray allocateData( VkImage src
			, VkImageCopy const & copyInfo )
		{
			auto dimensions = copyInfo.extent;
			auto texels = dimensions.width * dimensions.height * dimensions.depth;
			return ByteArray( texels * ashes::getSize( get( src )->getFormatVk() ), uint8_t( 0 ) );
		}

		void copyData( ashes::ByteArray const & srcData
			, VkImage const & src
			, VkImageCopy const & copyInfo
			, ByteArray & dstData )
		{
			auto dimensions = copyInfo.extent;
			auto texelSize = ashes::getSize( get( src )->getFormatVk() );
			auto dstRowSize = dimensions.width * texelSize;
			auto srcRowSize = get( src )->getDimensions().width * texelSize;
			auto planeSize = get( src )->getDimensions().height * srcRowSize;
			auto plane = srcData.data() + planeSize * copyInfo.srcOffset.z;
			auto buffer = dstData.data();

			for ( auto depth = 0u; depth < copyInfo.extent.depth; ++depth )
			{
				auto line = plane + srcRowSize * copyInfo.srcOffset.y;

				for ( auto height = 0u; height < copyInfo.extent.height; ++height )
				{
					std::memcpy( buffer, line + copyInfo.srcOffset.x * texelSize, dstRowSize );
					line += srcRowSize;
					buffer += dstRowSize;
				}

				plane += planeSize;
			}
		}

		ByteArray retrieveData( ContextLock const & context
			, VkImage src
			, VkImageCopy copy
			, GlTextureType srcTarget
			, GLuint srcName )
		{
			auto srcData = allocateData( src );
			auto dstData = allocateData( src, copy );
			glLogCall( context
				, glBindTexture
				, srcTarget
				, srcName );
			glLogCall( context
				, glGetTexImage
				, srcTarget
				, copy.srcSubresource.mipLevel
				, get( src )->getPackFormat()
				, get( src )->getPackType()
				, srcData.data() );
			glLogCall( context
				, glBindTexture
				, srcTarget
				, 0u );
			copyData( srcData
				, src
				, copy
				, dstData );
			return dstData;
		}

		VkBufferImageCopy getBufferImageCopy( VkImageCopy const & copyInfo
			, VkImage dstImage )
		{
			VkBufferImageCopy result{};
			auto & dstBinding = static_cast< ImageMemoryBinding const & >( get( dstImage )->getMemoryBinding() );
			result.bufferOffset = dstBinding.getMipLevelOffset( copyInfo.dstSubresource.mipLevel ) - dstBinding.getOffset();
			result.imageExtent = copyInfo.extent;
			result.imageOffset = copyInfo.dstOffset;
			result.imageSubresource = copyInfo.dstSubresource;
			return result;
		}
	}

	void buildCopyImageCommand( ContextStateStack & stack
		, VkDevice device
		, VkImageCopy copyInfo
		, VkImage srcImage
		, VkImage dstImage
		, CmdList & list )
	{
		glLogCommand( list, "CopyImageCommand" );
		assert( copyInfo.srcSubresource.layerCount == copyInfo.dstSubresource.layerCount
			|| copyInfo.srcSubresource.layerCount == copyInfo.extent.depth
			|| copyInfo.dstSubresource.layerCount == copyInfo.extent.depth );

		if ( hasCopyImage( device )
			&& areCopyCompatible( get( srcImage )->getFormatVk(), get( dstImage )->getFormatVk() ) )
		{
			auto srcTarget = convert( device
				, get( srcImage )->getType()
				, get( srcImage )->getArrayLayers()
				, get( srcImage )->getCreateFlags() );
			auto dstTarget = convert( device
				, get( dstImage )->getType()
				, get( dstImage )->getArrayLayers()
				, get( srcImage )->getCreateFlags() );
			list.push_back( makeCmd< OpType::eCopyImageSubData >( get( srcImage )->getInternal()
				, srcTarget
				, get( dstImage )->getInternal()
				, dstTarget
				, copyInfo ) );
		}
		else
		{
			auto layerCount = std::max( copyInfo.srcSubresource.layerCount
				, std::max( copyInfo.dstSubresource.layerCount
					, copyInfo.extent.depth ) );
			auto srcBaseArrayLayer = std::max( copyInfo.srcSubresource.baseArrayLayer
				, uint32_t( copyInfo.srcOffset.z ) );
			auto dstBaseArrayLayer = std::max( copyInfo.dstSubresource.baseArrayLayer
				, uint32_t( copyInfo.dstOffset.z ) );

			for ( uint32_t layer = 0u; layer < layerCount; ++layer )
			{
				LayerCopy layerCopy{ device
					, copyInfo
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
					, GL_FILTER_NEAREST ) );
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
			buildCopyImageToBufferCommand( stack
				, device
				, getBufferImageCopy( copyInfo, dstImage )
				, dstImage
				, get( dstImage )->getMemoryBinding()
				, list );
		}
	}
}
