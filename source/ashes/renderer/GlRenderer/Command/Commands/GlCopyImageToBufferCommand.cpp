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
			, GLuint dst
			, CmdList & list )
		{
			auto baseArrayLayer = std::max( srcCopyInfo.imageSubresource.baseArrayLayer
				, uint32_t( srcCopyInfo.imageOffset.z ) );

			auto copyInfo = srcCopyInfo;
			FboAttachment srcAttach{ device
				, copyInfo.imageSubresource
				, src };
			list.push_back( makeCmd< OpType::eBindBuffer >( GL_BUFFER_TARGET_PIXEL_PACK
				, dst ) );
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
				, get( src )->getPackFormat()
				, get( src )->getPackType() ) );
			list.push_back( makeCmd< OpType::eBindFramebuffer >( GL_READ_FRAMEBUFFER
				, nullptr ) );
			list.push_back( makeCmd< OpType::eBindBuffer >( GL_BUFFER_TARGET_PIXEL_PACK
				, 0u ) );

			if ( stack.hasCurrentFramebuffer() )
			{
				stack.setCurrentFramebuffer( nullptr );
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

			if ( isCompressedFormat( get( src )->getFormatVk() ) )
			{
				list.push_back( makeCmd< OpType::eGetCompressedTexImage >( get( src )->getTarget()
					, GLint( copyInfo.imageSubresource.mipLevel )
					, intptr_t( srcBufferOffset ) ) );
			}
			else
			{
				list.push_back( makeCmd< OpType::eGetTexImage >( get( src )->getTarget()
					, get( src )->getPackFormat()
					, get( src )->getPackType()
					, GLint( copyInfo.imageSubresource.mipLevel )
					, intptr_t( srcBufferOffset ) ) );
			}

			list.push_back( makeCmd< OpType::eBindTexture >( get( src )->getTarget(), 0u ) );
			list.push_back( makeCmd< OpType::eBindBuffer >( GL_BUFFER_TARGET_PIXEL_PACK, 0u ) );
			return srcBufferOffset;
		}

		VkImageViewType getLayerViewType( VkImageType src )
		{
			switch ( src )
			{
			case VK_IMAGE_TYPE_1D:
				return VK_IMAGE_VIEW_TYPE_1D;
			case VK_IMAGE_TYPE_2D:
				return VK_IMAGE_VIEW_TYPE_2D;
			case VK_IMAGE_TYPE_3D:
				return VK_IMAGE_VIEW_TYPE_3D;
			default:
				return VK_IMAGE_VIEW_TYPE_2D;
			}
		}

		VkImageViewCreateInfo getViewCreateInfo( VkImage src
			, VkBufferImageCopy copyInfo
			, uint32_t arrayLayer )
		{
			return VkImageViewCreateInfo{ VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO
				, nullptr
				, 0u
				, src
				, getLayerViewType( get( src )->getType() )
				, get( src )->getFormatVk()
				, VkComponentMapping{}
				, VkImageSubresourceRange{ copyInfo.imageSubresource.aspectMask
					, copyInfo.imageSubresource.mipLevel
					, 1u
					, copyInfo.imageSubresource.baseArrayLayer
					, copyInfo.imageSubresource.layerCount } };
		}

		void copyImageFullDataToImgBuffer( ContextStateStack & stack
			, VkBufferImageCopy copyInfo
			, VkImage src
			, ImageMemoryBinding const & srcBinding
			, CmdList & list )
		{
			auto bufferOffset = srcBinding.getMipLevelOffset( copyInfo.imageSubresource.mipLevel );
			auto layerSize = srcBinding.getArrayLayerSize();
			auto layerCount = std::max( copyInfo.imageSubresource.layerCount
				, copyInfo.imageExtent.depth );
			list.push_back( makeCmd< OpType::eBindBuffer >( GL_BUFFER_TARGET_PIXEL_PACK, get( srcBinding.getParent() )->getInternal() ) );
			stack.applyPackAlign( list, 1 );

			for ( uint32_t layer = 0u; layer < layerCount; ++layer )
			{
				VkImageView layerView = get( src )->createView( getViewCreateInfo( src, copyInfo, layer ) );
				list.push_back( makeCmd< OpType::eBindTexture >( get( layerView )->getTextureType(), get( src )->getInternal() ) );

				if ( isCompressedFormat( get( src )->getFormatVk() ) )
				{
					list.push_back( makeCmd< OpType::eGetCompressedTexImage >( get( layerView )->getTextureType()
						, GLint( copyInfo.imageSubresource.mipLevel )
						, intptr_t( bufferOffset ) ) );
				}
				else
				{
					list.push_back( makeCmd< OpType::eGetTexImage >( get( layerView )->getTextureType()
						, get( src )->getPackFormat()
						, get( src )->getPackType()
						, GLint( copyInfo.imageSubresource.mipLevel )
						, intptr_t( bufferOffset ) ) );
				}

				list.push_back( makeCmd< OpType::eBindTexture >( get( layerView )->getTextureType(), 0u ) );
				bufferOffset += layerSize;
			}

			list.push_back( makeCmd< OpType::eBindBuffer >( GL_BUFFER_TARGET_PIXEL_PACK, 0u ) );
			list.push_back( makeCmd< OpType::eDownloadMemory >( srcBinding.getParent()
				, srcBinding.getOffset()
				, srcBinding.getSize() ) );
		}

		void copyImageFullDataToBuffer( VkBufferImageCopy copyInfo
			, VkImage src
			, GLuint dst
			, VkDeviceSize srcBufferOffset
			, VkDeviceSize srcMipLayerSize
			, CmdList & list )
		{
			auto dstBufferOffset = copyInfo.bufferOffset;
			list.push_back( makeCmd< OpType::eBindBuffer >( GL_BUFFER_TARGET_COPY_READ
				, get( get( src )->getMemoryBinding().getParent() )->getInternal() ) );
			list.push_back( makeCmd< OpType::eBindBuffer >( GL_BUFFER_TARGET_COPY_WRITE
				, dst ) );
			auto layerCount = std::max( copyInfo.imageSubresource.layerCount
				, copyInfo.imageExtent.depth );

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
			, GLuint dst
			, VkExtent3D mipExtent
			, VkDeviceSize srcBufferOffset
			, VkDeviceSize srcMipLayerSize
			, CmdList & list )
		{
			auto dstBufferOffset = copyInfo.bufferOffset;
			auto srcRowSize = srcMipLayerSize / mipExtent.height;
			auto srcPixelSize = srcRowSize / mipExtent.width;
			mipExtent.width = std::min( mipExtent.width, copyInfo.imageExtent.width );
			mipExtent.height = std::min( mipExtent.height, copyInfo.imageExtent.height );
			mipExtent.depth = std::min( mipExtent.depth, copyInfo.imageExtent.depth );
			auto srcSubRowSize = srcPixelSize * mipExtent.width;
			auto dstRowWidth = ( copyInfo.bufferRowLength == 0 || copyInfo.bufferRowLength == copyInfo.imageExtent.width )
				? copyInfo.imageExtent.width
				: copyInfo.bufferRowLength;
			auto rowCount = ( copyInfo.bufferImageHeight == 0 || copyInfo.bufferImageHeight == copyInfo.imageExtent.height )
				? copyInfo.imageExtent.height
				: copyInfo.bufferImageHeight;
			auto dstRowSize = dstRowWidth * getMinimalSize( get( src )->getFormatVk() );
			auto dstMipLayerSize = dstRowSize * rowCount;
			auto layerCount = std::max( copyInfo.imageSubresource.layerCount
				, copyInfo.imageExtent.depth );

			list.push_back( makeCmd< OpType::eBindBuffer >( GL_BUFFER_TARGET_COPY_READ
				, get( get( src )->getMemoryBinding().getParent() )->getInternal() ) );
			list.push_back( makeCmd< OpType::eBindBuffer >( GL_BUFFER_TARGET_COPY_WRITE
				, dst ) );

			for ( uint32_t layer = 0u; layer < layerCount; ++layer )
			{
				auto srcRowOffset = srcBufferOffset + ( srcPixelSize * copyInfo.imageOffset.x );
				auto dstRowOffset = dstBufferOffset;

				for ( uint32_t row = 0u; row < rowCount; ++row )
				{
					list.push_back( makeCmd< OpType::eCopyBufferSubData >( GL_BUFFER_TARGET_COPY_READ
						, GL_BUFFER_TARGET_COPY_WRITE
						, VkBufferCopy{ srcRowOffset, dstRowOffset, srcSubRowSize } ) );

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
		, DeviceMemoryBinding const & dst
		, CmdList & list )
	{
		glLogCommand( list, "CopyImageToBufferCommand" );
		copyInfo.bufferOffset += dst.getOffset();
		auto layerCount = std::max( copyInfo.imageSubresource.layerCount
			, copyInfo.imageExtent.depth );
		auto & srcBinding = static_cast< ImageMemoryBinding const & >( get( src )->getMemoryBinding() );

		if ( get( src )->isReadSupported()
			&& layerCount == 1u
			&& !isCompressedFormat( get( src )->getFormatVk() ) )
		{
			readImagePixels( stack
				, device
				, copyInfo
				, src
				, get( dst.getParent() )->getInternal()
				, list );
		}
		else if ( &srcBinding == &dst )
		{
			copyImageFullDataToImgBuffer( stack
				, copyInfo
				, src
				, srcBinding
				, list );
		}
		else
		{
			auto srcBufferOffset = getTextureImage( stack, copyInfo, src, list );
			auto mipExtent = getSubresourceDimensions( get( src )->getDimensions(), copyInfo.imageSubresource.mipLevel );
			auto mipLayerSize = srcBinding.getMipLevelLayerSize( copyInfo.imageSubresource.mipLevel );
			auto baseArrayLayer = std::max( copyInfo.imageSubresource.baseArrayLayer
				, uint32_t( copyInfo.imageOffset.z ) );
			srcBufferOffset += mipLayerSize * baseArrayLayer;

			if ( mipExtent == copyInfo.imageExtent
				&& ( copyInfo.bufferRowLength == 0 || copyInfo.bufferRowLength == copyInfo.imageExtent.width )
				&& ( copyInfo.bufferImageHeight == 0 || copyInfo.bufferImageHeight == copyInfo.imageExtent.height ) )
			{
				copyImageFullDataToBuffer( copyInfo
					, src
					, get( dst.getParent() )->getInternal()
					, srcBufferOffset
					, mipLayerSize
					, list );
			}
			else
			{
				copyImagePartialDataToBuffer( copyInfo
					, src
					, get( dst.getParent() )->getInternal()
					, mipExtent
					, srcBufferOffset
					, mipLayerSize
					, list );
			}
		}
	}

	void buildCopyImageToBufferCommand( ContextStateStack & stack
		, VkDevice device
		, VkBufferImageCopy copyInfo
		, VkImage src
		, VkBuffer dst
		, CmdList & list )
	{
		buildCopyImageToBufferCommand( stack
			, device
			, copyInfo
			, src
			, get( dst )->getMemoryBinding()
			, list );
	}
}
