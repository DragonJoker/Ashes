/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlCopyImageToBufferCommand.hpp"

#include "Buffer/GlBuffer.hpp"
#include "Core/GlDevice.hpp"
#include "Image/GlImage.hpp"
#include "Image/GlImageView.hpp"
#include "Miscellaneous/GlCallLogger.hpp"
#include "Miscellaneous/GlImageMemoryBinding.hpp"

#include "ashesgl_api.hpp"

namespace ashes::gl
{
	namespace
	{
		void readImagePixels( ContextStateStack & stack
			, VkDevice device
			, VkBufferImageCopy srcCopyInfo
			, VkImage src
			, VkBuffer dst
			, CmdList & list
			, VkImageViewArray & views )
		{
			auto baseArrayLayer = std::max( srcCopyInfo.imageSubresource.baseArrayLayer
				, uint32_t( srcCopyInfo.imageOffset.z ) );

			auto copyInfo = srcCopyInfo;
			VkImageView srcView{ VK_NULL_HANDLE };
			FboAttachment srcAttach{ initialiseAttachment( device
				, copyInfo.imageSubresource
				, src
				, srcView ) };

			if ( srcView != VK_NULL_HANDLE )
			{
				views.push_back( srcView );
				copyInfo.imageSubresource.mipLevel = 0u;
			}

			list.push_back( makeCmd< OpType::eBindBuffer >( GL_BUFFER_TARGET_PIXEL_PACK
				, get( dst )->getInternal() ) );
			stack.applyPackAlign( list, 1 );
			list.push_back( makeCmd< OpType::eBindSrcFramebuffer >( GL_READ_FRAMEBUFFER ) );
			srcAttach.bindRead( stack
				, copyInfo.imageSubresource.mipLevel
				, baseArrayLayer
				, GL_READ_FRAMEBUFFER, list );
			list.push_back( makeCmd< OpType::eReadPixels >( copyInfo.imageOffset.x
				, copyInfo.imageOffset.y
				, copyInfo.imageExtent.width
				, copyInfo.imageExtent.height
				, get( src )->getReadFormat()
				, get( src )->getReadType() ) );
			list.push_back( makeCmd< OpType::eBindFramebuffer >( GL_READ_FRAMEBUFFER
				, nullptr ) );
			list.push_back( makeCmd< OpType::eBindBuffer >( GL_BUFFER_TARGET_PIXEL_PACK
				, 0u ) );

			if ( stack.hasCurrentFramebuffer() )
			{
				stack.setCurrentFramebuffer( VK_NULL_HANDLE );
			}
		}

		VkDeviceSize getTextureImage( ContextStateStack & stack
			, VkBufferImageCopy copyInfo
			, VkImage src
			, CmdList & list )
		{
			auto & srcBinding = static_cast< ImageMemoryBinding const & >( get( src )->getMemoryBinding() );
			auto srcBufferOffset = srcBinding.getMipLevelOffset( copyInfo.imageSubresource.mipLevel );
			list.push_back( makeCmd< OpType::eBindBuffer >( GL_BUFFER_TARGET_PIXEL_PACK, get( srcBinding.getParent() )->getInternal() ) );
			stack.applyPackAlign( list, 1 );
			list.push_back( makeCmd< OpType::eBindTexture >( get( src )->getTarget(), get( src )->getInternal() ) );
			list.push_back( makeCmd< OpType::eGetTexImage >( get( src )->getTarget()
				, get( src )->getGetFormat()
				, get( src )->getGetType()
				, GLint( copyInfo.imageSubresource.mipLevel )
				, intptr_t( srcBufferOffset ) ) );
			list.push_back( makeCmd< OpType::eBindTexture >( get( src )->getTarget(), 0u ) );
			list.push_back( makeCmd< OpType::eBindBuffer >( GL_BUFFER_TARGET_PIXEL_PACK, 0u ) );
			return srcBufferOffset;
		}

		void copyImageFullDataToBuffer( VkBufferImageCopy copyInfo
			, VkImage src
			, VkBuffer dst
			, VkDeviceSize srcBufferOffset
			, VkDeviceSize srcMipLayerSize
			, CmdList & list )
		{
			auto dstBufferOffset = copyInfo.bufferOffset;
			list.push_back( makeCmd< OpType::eBindBuffer >( GL_BUFFER_TARGET_COPY_READ
				, get( get( src )->getMemoryBinding().getParent() )->getInternal() ) );
			list.push_back( makeCmd< OpType::eBindBuffer >( GL_BUFFER_TARGET_COPY_WRITE
				, get( dst )->getInternal() ) );
			auto layerCount = std::max( copyInfo.imageSubresource.layerCount
				, copyInfo.imageExtent.depth );
			auto dstRowWidth = ( copyInfo.bufferRowLength == 0 || copyInfo.bufferRowLength == copyInfo.imageExtent.width )
				? copyInfo.imageExtent.width
				: copyInfo.bufferRowLength;

			for ( uint32_t layer = 0u; layer < layerCount; ++layer )
			{
				list.push_back( makeCmd< OpType::eCopyBufferSubData >( GL_BUFFER_TARGET_COPY_READ
					, GL_BUFFER_TARGET_COPY_WRITE
					, VkBufferCopy{ srcBufferOffset, dstBufferOffset, srcMipLayerSize } ) );
				srcBufferOffset += srcMipLayerSize;
				dstBufferOffset += srcMipLayerSize;
			}

			list.push_back( makeCmd< OpType::eBindBuffer >( GL_BUFFER_TARGET_COPY_WRITE
				, 0u ) );
			list.push_back( makeCmd< OpType::eBindBuffer >( GL_BUFFER_TARGET_COPY_READ
				, 0u ) );
		}

		void copyImagePartialDataToBuffer( VkBufferImageCopy copyInfo
			, VkImage src
			, VkBuffer dst
			, VkExtent3D mipExtent
			, VkDeviceSize srcBufferOffset
			, VkDeviceSize srcMipLayerSize
			, CmdList & list )
		{
			auto dstBufferOffset = copyInfo.bufferOffset;
			mipExtent.width = std::min( mipExtent.width, copyInfo.imageExtent.width );
			mipExtent.height = std::min( mipExtent.height, copyInfo.imageExtent.height );
			mipExtent.depth = std::min( mipExtent.depth, copyInfo.imageExtent.depth );
			auto dstRowWidth = ( copyInfo.bufferRowLength == 0 || copyInfo.bufferRowLength == copyInfo.imageExtent.width )
				? copyInfo.imageExtent.width
				: copyInfo.bufferRowLength;
			auto rowCount = ( copyInfo.bufferImageHeight == 0 || copyInfo.bufferImageHeight == copyInfo.imageExtent.height )
				? copyInfo.imageExtent.height
				: copyInfo.bufferImageHeight;
			auto srcRowSize = srcMipLayerSize / mipExtent.height;
			auto dstRowSize = dstRowWidth * getMinimalSize( get( src )->getFormatVk() );
			auto dstMipLayerSize = dstRowSize * rowCount;
			auto layerCount = std::max( copyInfo.imageSubresource.layerCount
				, copyInfo.imageExtent.depth );

			list.push_back( makeCmd< OpType::eBindBuffer >( GL_BUFFER_TARGET_COPY_READ
				, get( get( src )->getMemoryBinding().getParent() )->getInternal() ) );
			list.push_back( makeCmd< OpType::eBindBuffer >( GL_BUFFER_TARGET_COPY_WRITE
				, get( dst )->getInternal() ) );

			for ( uint32_t layer = 0u; layer < layerCount; ++layer )
			{
				auto srcRowOffset = srcBufferOffset + ( srcRowSize * copyInfo.imageOffset.x );
				auto dstRowOffset = dstBufferOffset;

				for ( uint32_t row = 0u; row < rowCount; ++row )
				{
					list.push_back( makeCmd< OpType::eCopyBufferSubData >( GL_BUFFER_TARGET_COPY_READ
						, GL_BUFFER_TARGET_COPY_WRITE
						, VkBufferCopy{ srcRowOffset, dstRowOffset, srcRowSize } ) );

					srcRowOffset += srcRowSize;
					dstRowOffset += dstRowSize;
				}

				srcBufferOffset += srcMipLayerSize;
				dstBufferOffset += dstMipLayerSize;
			}

			list.push_back( makeCmd< OpType::eBindBuffer >( GL_BUFFER_TARGET_COPY_WRITE
				, 0u ) );
			list.push_back( makeCmd< OpType::eBindBuffer >( GL_BUFFER_TARGET_COPY_READ
				, 0u ) );
		}
	}

	void buildCopyImageToBufferCommand( ContextStateStack & stack
		, VkDevice device
		, VkBufferImageCopy copyInfo
		, VkImage src
		, VkBuffer dst
		, CmdList & list
		, VkImageViewArray & views )
	{
		glLogCommand( list, "CopyImageToBufferCommand" );
		copyInfo.bufferOffset += get( dst )->getMemoryBinding().getOffset();
		auto layerCount = std::max( copyInfo.imageSubresource.layerCount
			, copyInfo.imageExtent.depth );

		if ( get( src )->isReadSupported()
			&& layerCount == 1u )
		{
			readImagePixels( stack, device, copyInfo, src, dst, list, views );
		}
		else
		{
			auto srcBufferOffset = getTextureImage( stack, copyInfo, src, list );
			auto & srcBinding = static_cast< ImageMemoryBinding const & >( get( src )->getMemoryBinding() );
			auto mipExtent = getSubresourceDimensions( get( src )->getDimensions(), copyInfo.imageSubresource.mipLevel );
			auto mipLayerSize = srcBinding.getMipLevelLayerSize( copyInfo.imageSubresource.mipLevel );
			auto baseArrayLayer = std::max( copyInfo.imageSubresource.baseArrayLayer
				, uint32_t( copyInfo.imageOffset.z ) );
			srcBufferOffset += mipLayerSize * baseArrayLayer;

			if ( mipExtent == copyInfo.imageExtent
				&& ( copyInfo.bufferRowLength == 0 || copyInfo.bufferRowLength == copyInfo.imageExtent.width )
				&& ( copyInfo.bufferImageHeight == 0 || copyInfo.bufferImageHeight == copyInfo.imageExtent.height ) )
			{
				copyImageFullDataToBuffer( copyInfo, src, dst, srcBufferOffset, mipLayerSize, list );
			}
			else
			{
				copyImagePartialDataToBuffer( copyInfo, src, dst, mipExtent, srcBufferOffset, mipLayerSize, list );
			}
		}
	}
}
