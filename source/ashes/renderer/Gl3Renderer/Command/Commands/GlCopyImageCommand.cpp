/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlCopyImageCommand.hpp"

#include "Image/GlImage.hpp"
#include "Image/GlImageView.hpp"

#include "ashesgl3_api.hpp"

#include <cstring>

namespace ashes::gl3
{
	namespace
	{
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
	}

	ByteArray retrieveData( ContextLock const & context
		, VkImage src
		, VkImageCopy copy
		, uint32_t srcTarget
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
			, srcInternal
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
			, srcInternal
			, getType( srcInternal )
			, dstData.data() );
		glLogCall( context
			, glGenerateMipmap
			, cmd.dstTarget );
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
			, cmd.dstTarget
			, cmd.copy.dstSubresource.mipLevel
			, cmd.copy.dstOffset.x
			, cmd.dstOffsetY
			, cmd.copy.extent.width
			, cmd.dstExtentY
			, srcInternal
			, getType( srcInternal )
			, dstData.data() );
		glLogCall( context
			, glGenerateMipmap
			, cmd.dstTarget );
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
			, cmd.dstTarget
			, cmd.copy.dstSubresource.mipLevel
			, cmd.copy.dstOffset.x
			, cmd.copy.dstOffset.y
			, cmd.dstOffsetZ
			, cmd.copy.extent.width
			, cmd.copy.extent.height
			, cmd.dstExtentZ
			, srcInternal
			, getType( srcInternal )
			, dstData.data() );
		glLogCall( context
			, glGenerateMipmap
			, cmd.dstTarget );
		glLogCall( context
			, glBindTexture
			, cmd.dstTarget
			, 0u );
	}

	void buildCopyImageCommand( VkImageCopy copyInfo
		, VkImage src
		, VkImage dst
		, CmdList & list )
	{
		glLogCommand( "CopyImageCommand" );
		auto srcTarget = convert( get( src )->getType()
			, get( src )->getArrayLayers()
			, get( src )->getCreateFlags() );
		auto dstTarget = convert( get( dst )->getType()
			, get( dst )->getArrayLayers()
			, get( dst )->getCreateFlags() );

		switch ( dstTarget )
		{
		case GL_TEXTURE_1D:
			list.push_back( makeCmd< OpType::eCopyImageSubData1D >( src
				, get( src )->getInternal()
				, srcTarget
				, dst
				, get( dst )->getInternal()
				, dstTarget
				, std::move( copyInfo ) ) );
			break;

		case GL_TEXTURE_1D_ARRAY:
			list.push_back( makeCmd< OpType::eCopyImageSubData2D >( src
				, get( src )->getInternal()
				, srcTarget
				, dst
				, get( dst )->getInternal()
				, dstTarget
				, int32_t( copyInfo.dstSubresource.baseArrayLayer )
				, copyInfo.dstSubresource.layerCount
				, std::move( copyInfo ) ) );
			break;

		case GL_TEXTURE_2D:
			list.push_back( makeCmd< OpType::eCopyImageSubData2D >( src
				, get( src )->getInternal()
				, srcTarget
				, dst
				, get( dst )->getInternal()
				, dstTarget
				, copyInfo.dstOffset.y
				, copyInfo.extent.height
				, std::move( copyInfo ) ) );
			break;

		case GL_TEXTURE_2D_ARRAY:
			list.push_back( makeCmd< OpType::eCopyImageSubData3D >( src
				, get( src )->getInternal()
				, srcTarget
				, dst
				, get( dst )->getInternal()
				, dstTarget
				, int32_t( copyInfo.dstSubresource.baseArrayLayer )
				, copyInfo.dstSubresource.layerCount
				, std::move( copyInfo ) ) );
			break;

		case GL_TEXTURE_CUBE:
			list.push_back( makeCmd< OpType::eCopyImageSubData2D >( src
				, get( src )->getInternal()
				, srcTarget
				, dst
				, get( dst )->getInternal()
				, GL_TEXTURE_CUBE_POSITIVE_X + copyInfo.dstSubresource.baseArrayLayer
				, copyInfo.dstOffset.y
				, copyInfo.extent.height
				, std::move( copyInfo ) ) );
			break;

		case GL_TEXTURE_CUBE_ARRAY:
			list.push_back( makeCmd< OpType::eCopyImageSubData3D >( src
				, get( src )->getInternal()
				, srcTarget
				, dst
				, get( dst )->getInternal()
				, GL_TEXTURE_CUBE_POSITIVE_X + ( copyInfo.dstSubresource.baseArrayLayer % 6u )
				, int32_t( copyInfo.dstSubresource.baseArrayLayer / 6u )
				, copyInfo.dstSubresource.layerCount / 6u
				, std::move( copyInfo ) ) );
			break;

		case GL_TEXTURE_3D:
			list.push_back( makeCmd< OpType::eCopyImageSubData3D >( src
				, get( src )->getInternal()
				, srcTarget
				, dst
				, get( dst )->getInternal()
				, dstTarget
				, copyInfo.dstOffset.z
				, copyInfo.extent.depth
				, std::move( copyInfo ) ) );
			break;

		default:
			std::cerr << "CopyImageCommand - Unsupported texture type." << std::endl;
			break;
		}
	}
}
