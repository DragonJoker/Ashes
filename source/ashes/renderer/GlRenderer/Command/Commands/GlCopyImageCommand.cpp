/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlCopyImageCommand.hpp"
#include "Command/Commands/GlBlitImageCommand.hpp"

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
			return ByteArray( texels * ashes::getSize( get( src )->getFormat() ), uint8_t( 0 ) );
		}

		ByteArray allocateData( VkImage src
			, VkImageCopy const & copyInfo )
		{
			auto dimensions = copyInfo.extent;
			auto texels = dimensions.width * dimensions.height * dimensions.depth;
			return ByteArray( texels * ashes::getSize( get( src )->getFormat() ), uint8_t( 0 ) );
		}

		void copyData( ashes::ByteArray const & srcData
			, VkImage const & src
			, VkImageCopy const & copyInfo
			, ByteArray & dstData )
		{
			auto dimensions = copyInfo.extent;
			auto texelSize = ashes::getSize( get( src )->getFormat() );
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
			auto srcInternal{ getInternalFormat( get( src )->getFormat() ) };
			glLogCall( context
				, glBindTexture
				, srcTarget
				, srcName );
			glLogCall( context
				, glGetTexImage
				, srcTarget
				, copy.srcSubresource.mipLevel
				, getFormat( srcInternal )
				, getType( srcInternal )
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
	}

	void apply( ContextLock const & context
		, CmdCopyImageSubData const & cmd )
	{
		glLogCall( context
			, glCopyImageSubData
			, cmd.srcName
			, cmd.srcTarget
			, cmd.copy.srcSubresource.mipLevel
			, cmd.copy.srcOffset.x
			, cmd.copy.srcOffset.y
			, ( cmd.copy.srcSubresource.baseArrayLayer
				? GLint( cmd.copy.srcSubresource.baseArrayLayer )
				: GLint( cmd.copy.srcOffset.z ) )
			, cmd.dstName
			, cmd.dstTarget
			, cmd.copy.dstSubresource.mipLevel
			, cmd.copy.dstOffset.x
			, cmd.copy.dstOffset.y
			, ( cmd.copy.dstSubresource.baseArrayLayer
				? GLint( cmd.copy.dstSubresource.baseArrayLayer )
				: GLint( cmd.copy.dstOffset.z ) )
			, GLsizei( cmd.copy.extent.width )
			, GLsizei( cmd.copy.extent.height )
			, ( cmd.copy.srcSubresource.layerCount > 1u
				? GLsizei( cmd.copy.srcSubresource.layerCount )
				: GLsizei( cmd.copy.extent.depth ) ) );
	}

	void apply( ContextLock const & context
		, CmdCopyImageSubData1D const & cmd )
	{
		auto srcInternal{ getInternalFormat( get( cmd.src )->getFormat() ) };
		auto dstData = retrieveData( context
			, cmd.src
			, cmd.copy
			, cmd.srcTarget
			, cmd.srcName );
		glLogCall( context
			, glBindTexture
			, cmd.dstTarget
			, cmd.dstName );
		glLogCall( context
			, glTexSubImage1D
			, cmd.dstTarget
			, cmd.copy.dstSubresource.mipLevel
			, cmd.copy.dstOffset.x
			, cmd.copy.extent.width
			, getFormat( srcInternal )
			, getType( srcInternal )
			, dstData.data() );
		glLogCall( context
			, glBindTexture
			, cmd.dstTarget
			, 0u );
	}

	void apply( ContextLock const & context
		, CmdCopyImageSubData2D const & cmd )
	{
		auto srcInternal{ getInternalFormat( get( cmd.src )->getFormat() ) };
		auto dstData = retrieveData( context
			, cmd.src
			, cmd.copy
			, cmd.srcTarget
			, cmd.srcName );
		glLogCall( context
			, glBindTexture
			, cmd.dstTarget
			, cmd.dstName );
		glLogCall( context
			, glTexSubImage2D
			, cmd.dstLayerTarget
			, cmd.copy.dstSubresource.mipLevel
			, cmd.copy.dstOffset.x
			, cmd.dstOffsetY
			, cmd.copy.extent.width
			, cmd.dstExtentY
			, getFormat( srcInternal )
			, getType( srcInternal )
			, dstData.data() );
		glLogCall( context
			, glBindTexture
			, cmd.dstTarget
			, 0u );
	}

	void apply( ContextLock const & context
		, CmdCopyImageSubData3D const & cmd )
	{
		auto srcInternal{ getInternalFormat( get( cmd.src )->getFormat() ) };
		auto dstData = retrieveData( context
			, cmd.src
			, cmd.copy
			, cmd.srcTarget
			, cmd.srcName );
		glLogCall( context
			, glBindTexture
			, cmd.dstTarget
			, cmd.dstName );
		glLogCall( context
			, glTexSubImage3D
			, cmd.dstLayerTarget
			, cmd.copy.dstSubresource.mipLevel
			, cmd.copy.dstOffset.x
			, cmd.copy.dstOffset.y
			, cmd.dstOffsetZ
			, cmd.copy.extent.width
			, cmd.copy.extent.height
			, cmd.dstExtentZ
			, getFormat( srcInternal )
			, getType( srcInternal )
			, dstData.data() );
		glLogCall( context
			, glBindTexture
			, cmd.dstTarget
			, 0u );
	}

	void apply( ContextLock const & context
		, CmdGetTexImage const & cmd )
	{
		glLogCall( context
			, glGetTexImage
			, cmd.target
			, 0
			, cmd.format
			, cmd.type
			, getBufferOffset( 0 ) );
	}

	void buildCopyImageCommand( ContextStateStack & stack
		, VkDevice device
		, VkImageCopy copyInfo
		, VkImage srcImage
		, VkImage dstImage
		, CmdList & list )
	{
		glLogCommand( list, "CopyImageCommand" );

		if ( copyInfo.srcSubresource.mipLevel == copyInfo.dstSubresource.mipLevel
			&& hasCopyImage( device ) )
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
				, std::move( copyInfo ) ) );
		}
		else
		{
			// Setup source FBO
			list.push_back( makeCmd< OpType::eBindSrcFramebuffer >( GL_FRAMEBUFFER ) );

			if ( get( srcImage )->getArrayLayers() > 1u )
			{
				list.push_back( makeCmd< OpType::eFramebufferTextureLayer >( GL_FRAMEBUFFER
					, getAttachmentPoint( get( srcImage )->getFormat() )
					, get( srcImage )->getInternal()
					, copyInfo.srcSubresource.mipLevel
					, copyInfo.srcSubresource.baseArrayLayer ) );
			}
			else
			{
				list.push_back( makeCmd< OpType::eFramebufferTexture2D >( GL_FRAMEBUFFER
					, getAttachmentPoint( get( srcImage )->getFormat() )
					, get( srcImage )->getSamples() > VK_SAMPLE_COUNT_1_BIT ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D
					, get( srcImage )->getInternal()
					, copyInfo.srcSubresource.mipLevel ) );
			}

			list.push_back( makeCmd< OpType::eBindFramebuffer >( GL_FRAMEBUFFER
				, nullptr ) );

			// Setup dst FBO
			list.push_back( makeCmd< OpType::eBindDstFramebuffer >( GL_FRAMEBUFFER ) );

			if ( get( dstImage )->getArrayLayers() > 1u )
			{
				list.push_back( makeCmd< OpType::eFramebufferTextureLayer >( GL_FRAMEBUFFER
					, getAttachmentPoint( get( dstImage )->getFormat() )
					, get( dstImage )->getInternal()
					, copyInfo.dstSubresource.mipLevel
					, copyInfo.dstSubresource.baseArrayLayer ) );
			}
			else
			{
				list.push_back( makeCmd< OpType::eFramebufferTexture2D >( GL_FRAMEBUFFER
					, getAttachmentPoint( get( dstImage )->getFormat() )
					, get( dstImage )->getSamples() > VK_SAMPLE_COUNT_1_BIT ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D
					, get( dstImage )->getInternal()
					, copyInfo.dstSubresource.mipLevel ) );
			}

			list.push_back( makeCmd< OpType::eBindFramebuffer >( GL_FRAMEBUFFER
				, nullptr ) );

			// Perform the blit
			list.push_back( makeCmd< OpType::eBindSrcFramebuffer >( GL_READ_FRAMEBUFFER ) );
			list.push_back( makeCmd< OpType::eReadBuffer >( uint32_t( getAttachmentPoint( get( srcImage )->getFormat() ) ) ) );
			list.push_back( makeCmd< OpType::eBindDstFramebuffer >( GL_DRAW_FRAMEBUFFER ) );
			list.push_back( makeCmd< OpType::eDrawBuffers >( uint32_t( getAttachmentPoint( get( dstImage )->getFormat() ) ) ) );

			if ( copyInfo.dstSubresource.mipLevel > copyInfo.srcSubresource.mipLevel )
			{
				auto diff = copyInfo.dstSubresource.mipLevel - copyInfo.srcSubresource.mipLevel;
				list.push_back( makeCmd< OpType::eBlitFramebuffer >( copyInfo.srcOffset.x
					, copyInfo.srcOffset.y
					, int32_t( copyInfo.extent.width << diff )
					, int32_t( copyInfo.extent.height << diff )
					, copyInfo.dstOffset.x
					, copyInfo.dstOffset.y
					, int32_t( copyInfo.extent.width )
					, int32_t( copyInfo.extent.height )
					, getMask( get( srcImage )->getFormat() )
					, GL_FILTER_LINEAR ) );
			}
			else
			{
				auto diff = copyInfo.srcSubresource.mipLevel - copyInfo.dstSubresource.mipLevel;
				list.push_back( makeCmd< OpType::eBlitFramebuffer >( copyInfo.srcOffset.x
					, copyInfo.srcOffset.y
					, int32_t( copyInfo.extent.width )
					, int32_t( copyInfo.extent.height )
					, copyInfo.dstOffset.x
					, copyInfo.dstOffset.y
					, int32_t( copyInfo.extent.width << diff )
					, int32_t( copyInfo.extent.height << diff )
					, getMask( get( srcImage )->getFormat() )
					, GL_FILTER_LINEAR ) );
			}

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
