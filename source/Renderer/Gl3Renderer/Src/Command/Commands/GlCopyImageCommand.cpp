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
		renderer::ByteArray allocateData( renderer::Texture const & src )
		{
			auto dimensions = src.getDimensions();
			auto texels = dimensions.width * dimensions.height * dimensions.depth;
			return renderer::ByteArray( texels * renderer::getSize( src.getFormat() ), uint8_t( 0 ) );
		}

		renderer::ByteArray allocateData( renderer::Texture const & src
			, renderer::ImageCopy const & copyInfo )
		{
			auto dimensions = copyInfo.extent;
			auto texels = dimensions.width * dimensions.height * dimensions.depth;
			return renderer::ByteArray( texels * renderer::getSize( src.getFormat() ), uint8_t( 0 ) );
		}

		void copyData( renderer::ByteArray const & srcData
			, renderer::Texture const & src
			, renderer::ImageCopy const & copyInfo
			, renderer::ByteArray & dstData )
		{
			auto dimensions = copyInfo.extent;
			auto texelSize = renderer::getSize( src.getFormat() );
			auto dstRowSize = dimensions.width * texelSize;
			auto srcRowSize = src.getDimensions().width * texelSize;
			auto planeSize = src.getDimensions().height * srcRowSize * texelSize;
			auto plane = srcData.data() + planeSize * copyInfo.srcOffset.z;
			auto buffer = dstData.data();

			for ( auto depth = 0u; depth < copyInfo.extent.depth; ++depth )
			{
				auto line = plane + srcRowSize * copyInfo.srcOffset.y;

				for ( auto height = 0u; height < copyInfo.extent.height; ++height )
				{
					std::memcpy( buffer, line, dstRowSize );
					line += srcRowSize;
					buffer += dstRowSize;
				}

				plane += planeSize;
			}
		}
	}

	CopyImageCommand::CopyImageCommand( renderer::ImageCopy const & copyInfo
		, renderer::Texture const & src
		, renderer::Texture const & dst )
		: m_copyInfo{ copyInfo }
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

	void CopyImageCommand::apply()const
	{
		glLogCommand( "CopyImageCommand" );
		glLogCall( gl::BindTexture, m_srcTarget, m_src.getImage() );
		glLogCall( gl::GetTexImage
			, m_srcTarget
			, m_copyInfo.srcSubresource.mipLevel
			, m_srcFormat
			, m_srcType
			, m_srcData.data() );
		glLogCall( gl::BindTexture, m_srcTarget, 0u );
		glLogCall( gl::BindTexture, m_dstTarget, m_dst.getImage() );
		copyData( m_srcData
			, m_src
			, m_copyInfo
			, m_dstData );

		switch ( m_dstTarget )
		{
		case GL_TEXTURE_1D:
			glLogCall( gl::TexSubImage1D
				, m_srcTarget
				, m_copyInfo.srcSubresource.mipLevel
				, m_copyInfo.dstOffset.x
				, m_copyInfo.extent.width
				, m_srcFormat
				, m_srcType
				, m_dstData.data() );
			break;

		case GL_TEXTURE_1D_ARRAY:
			glLogCall( gl::TexSubImage2D
				, m_srcTarget
				, m_copyInfo.srcSubresource.mipLevel
				, m_copyInfo.dstOffset.x
				, m_copyInfo.dstSubresource.baseArrayLayer
				, m_copyInfo.extent.width
				, m_copyInfo.extent.height
				, m_srcFormat
				, m_srcType
				, m_dstData.data() );
			break;

		case GL_TEXTURE_2D:
			glLogCall( gl::TexSubImage2D
				, m_srcTarget
				, m_copyInfo.srcSubresource.mipLevel
				, m_copyInfo.dstOffset.x
				, m_copyInfo.dstOffset.y
				, m_copyInfo.extent.width
				, m_copyInfo.extent.height
				, m_srcFormat
				, m_srcType
				, m_dstData.data() );
			break;

		case GL_TEXTURE_2D_ARRAY:
		case GL_TEXTURE_CUBE:
		case GL_TEXTURE_CUBE_ARRAY:
			glLogCall( gl::TexSubImage3D
				, m_srcTarget
				, m_copyInfo.srcSubresource.mipLevel
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

		case GL_TEXTURE_3D:
			glLogCall( gl::TexSubImage3D
				, m_srcTarget
				, m_copyInfo.srcSubresource.mipLevel
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
			renderer::Logger::logError( "Unsupported texture type." );
			break;
		}

		glLogCall( gl::BindTexture, m_dstTarget, 0u );
		m_dst.generateMipmaps();
	}

	CommandPtr CopyImageCommand::clone()const
	{
		return std::make_unique< CopyImageCommand >( *this );
	}
}
