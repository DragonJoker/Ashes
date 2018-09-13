/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "GlCopyImageCommand.hpp"

#include "Image/GlTexture.hpp"
#include "Image/GlTextureView.hpp"

#include <Image/ImageSubresourceRange.hpp>

namespace gl_renderer
{
	namespace
	{
		ashes::ByteArray allocateData( ashes::Texture const & src )
		{
			auto dimensions = src.getDimensions();
			auto texels = dimensions.width * dimensions.height * dimensions.depth;
			return ashes::ByteArray( texels * ashes::getSize( src.getFormat() ), uint8_t( 0 ) );
		}

		ashes::ByteArray allocateData( ashes::Texture const & src
			, ashes::ImageCopy const & copyInfo )
		{
			auto dimensions = copyInfo.extent;
			auto texels = dimensions.width * dimensions.height * dimensions.depth;
			return ashes::ByteArray( texels * ashes::getSize( src.getFormat() ), uint8_t( 0 ) );
		}

		void copyData( ashes::ByteArray const & srcData
			, ashes::Texture const & src
			, ashes::ImageCopy const & copyInfo
			, ashes::ByteArray & dstData )
		{
			auto dimensions = copyInfo.extent;
			auto texelSize = ashes::getSize( src.getFormat() );
			auto dstRowSize = dimensions.width * texelSize;
			auto srcRowSize = src.getDimensions().width * texelSize;
			auto planeSize = src.getDimensions().height * srcRowSize;
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

	CopyImageCommand::CopyImageCommand( Device const & device
		, ashes::ImageCopy const & copyInfo
		, ashes::Texture const & src
		, ashes::Texture const & dst )
		: CommandBase{ device }
		, m_copyInfo{ copyInfo }
		, m_src{ static_cast< Texture const & >( src ) }
		, m_dst{ static_cast< Texture const & >( dst ) }
		, m_srcInternal{ getInternal( m_src.getFormat() ) }
		, m_srcFormat{ getFormat( m_srcInternal ) }
		, m_srcType{ getType( m_srcInternal ) }
		, m_srcTarget{ convert( m_src.getType(), m_src.getLayerCount(), m_src.getFlags() ) }
		, m_dstInternal{ getInternal( m_dst.getFormat() ) }
		, m_dstFormat{ getFormat( m_dstInternal ) }
		, m_dstType{ getType( m_dstInternal ) }
		, m_dstTarget{ convert( m_dst.getType(), m_dst.getLayerCount(), m_dst.getFlags() ) }
		, m_srcData{ allocateData( src ) }
		, m_dstData{ allocateData( src, m_copyInfo ) }
	{
	}

	void CopyImageCommand::apply( ContextLock const & context )const
	{
		glLogCommand( "CopyImageCommand" );
		glLogCall( context
			, glBindTexture
			, m_srcTarget
			, m_src.getImage() );
		glLogCall( context
			, glGetTexImage
			, m_srcTarget
			, m_copyInfo.srcSubresource.mipLevel
			, m_srcFormat
			, m_srcType
			, m_srcData.data() );
		glLogCall( context
			, glBindTexture
			, m_srcTarget
			, 0u );
		glLogCall( context
			, glBindTexture
			, m_dstTarget
			, m_dst.getImage() );
		copyData( m_srcData
			, m_src
			, m_copyInfo
			, m_dstData );

		switch ( m_dstTarget )
		{
		case GL_TEXTURE_1D:
			glLogCall( context
				, glTexSubImage1D
				, m_srcTarget
				, m_copyInfo.dstSubresource.mipLevel
				, m_copyInfo.dstOffset.x
				, m_copyInfo.extent.width
				, m_srcFormat
				, m_srcType
				, m_dstData.data() );
			break;

		case GL_TEXTURE_1D_ARRAY:
			glLogCall( context
				, glTexSubImage2D
				, m_srcTarget
				, m_copyInfo.dstSubresource.mipLevel
				, m_copyInfo.dstOffset.x
				, m_copyInfo.dstSubresource.baseArrayLayer
				, m_copyInfo.extent.width
				, m_copyInfo.extent.height
				, m_srcFormat
				, m_srcType
				, m_dstData.data() );
			break;

		case GL_TEXTURE_2D:
			glLogCall( context
				, glTexSubImage2D
				, m_srcTarget
				, m_copyInfo.dstSubresource.mipLevel
				, m_copyInfo.dstOffset.x
				, m_copyInfo.dstOffset.y
				, m_copyInfo.extent.width
				, m_copyInfo.extent.height
				, m_srcFormat
				, m_srcType
				, m_dstData.data() );
			break;

		case GL_TEXTURE_2D_ARRAY:
			glLogCall( context
				, glTexSubImage3D
				, m_srcTarget
				, m_copyInfo.dstSubresource.mipLevel
				, m_copyInfo.dstOffset.x
				, m_copyInfo.dstOffset.y
				, m_copyInfo.dstSubresource.baseArrayLayer
				, m_copyInfo.extent.width
				, m_copyInfo.extent.height
				, m_copyInfo.extent.depth
				, m_srcFormat
				, m_srcType
				, m_dstData.data() );
			break;

		case GL_TEXTURE_CUBE:
			glLogCall( context
				, glTexSubImage2D
				, GL_TEXTURE_CUBE_POSITIVE_X + m_copyInfo.dstSubresource.baseArrayLayer
				, m_copyInfo.dstSubresource.mipLevel
				, m_copyInfo.dstOffset.x
				, m_copyInfo.dstOffset.y
				, m_copyInfo.extent.width
				, m_copyInfo.extent.height
				, m_srcFormat
				, m_srcType
				, m_dstData.data() );
			break;

		case GL_TEXTURE_CUBE_ARRAY:
			glLogCall( context
				, glTexSubImage3D
				, GL_TEXTURE_CUBE_POSITIVE_X + ( m_copyInfo.dstSubresource.baseArrayLayer % 6u )
				, m_copyInfo.dstSubresource.mipLevel
				, m_copyInfo.dstOffset.x
				, m_copyInfo.dstOffset.y
				, m_copyInfo.dstSubresource.baseArrayLayer / 6u
				, m_copyInfo.extent.width
				, m_copyInfo.extent.height
				, m_copyInfo.dstSubresource.layerCount / 6u
				, m_srcFormat
				, m_srcType
				, m_dstData.data() );
			break;

		case GL_TEXTURE_3D:
			glLogCall( context
				, glTexSubImage3D
				, m_srcTarget
				, m_copyInfo.dstSubresource.mipLevel
				, m_copyInfo.dstOffset.x
				, m_copyInfo.dstOffset.y
				, m_copyInfo.dstOffset.z
				, m_copyInfo.extent.width
				, m_copyInfo.extent.height
				, m_copyInfo.extent.depth
				, m_srcFormat
				, m_srcType
				, m_dstData.data() );
			break;

		default:
			ashes::Logger::logError( "Unsupported texture type." );
			break;
		}

		glLogCall( context
			, glBindTexture
			, m_dstTarget
			, 0u );
		static_cast< ashes::Texture const & >( m_dst ).generateMipmaps();
	}

	CommandPtr CopyImageCommand::clone()const
	{
		return std::make_unique< CopyImageCommand >( *this );
	}
}
