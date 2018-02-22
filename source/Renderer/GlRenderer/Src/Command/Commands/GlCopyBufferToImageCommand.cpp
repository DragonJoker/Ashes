/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "GlCopyBufferToImageCommand.hpp"

#include "Buffer/GlBuffer.hpp"
#include "Image/GlTexture.hpp"
#include "Image/GlTextureView.hpp"

#include <Image/ImageSubresourceRange.hpp>

#define BufferOffset( n ) ( ( uint8_t * )nullptr + ( n ) )

namespace gl_renderer
{
	namespace
	{
		GlTextureType convert( renderer::TextureType type
			, uint32_t layer )
		{
			if ( type == renderer::TextureType::eCube )
			{
				return GlTextureType( GL_TEXTURE_CUBE_MAP_POSITIVE_X + layer );
			}
			else if ( type == renderer::TextureType::e2DArray )
			{
				return GL_TEXTURE_2D;
			}
			else if ( type == renderer::TextureType::e1DArray )
			{
				return GL_TEXTURE_1D;
			}

			return gl_renderer::convert( type );
		}

		std::vector< GlTextureType > convert( renderer::TextureType type
			, renderer::BufferImageCopyArray copies )
		{
			std::vector< GlTextureType > result;

			for ( auto & copy : copies )
			{
				result.push_back( convert( type, copy.imageSubresource.baseArrayLayer ) );
			}

			return result;
		}

		std::vector< renderer::TextureViewPtr > createViews( renderer::Texture const & texture
			, renderer::BufferImageCopyArray copies )
		{
			std::vector< renderer::TextureViewPtr > result;
			renderer::TextureType viewType = texture.getType();

			if ( viewType == renderer::TextureType::eCube
				|| viewType == renderer::TextureType::e2DArray )
			{
				viewType = renderer::TextureType::e2D;
			}
			else if ( viewType == renderer::TextureType::e1DArray )
			{
				viewType = renderer::TextureType::e1D;
			}

			for ( auto & copy : copies )
			{
				result.push_back( texture.createView( viewType
					, texture.getFormat()
					, copy.imageSubresource.mipLevel
					, 1u
					, copy.imageSubresource.baseArrayLayer
					, copy.imageSubresource.layerCount ) );
			}

			return result;
		}

		float getCompressedFormatSize( renderer::PixelFormat format )
		{
			switch ( format )
			{
			case renderer::PixelFormat::eBC1_RGB:
			case renderer::PixelFormat::eBC1_RGBA:
			case renderer::PixelFormat::eBC1_SRGB:
			case renderer::PixelFormat::eBC1_SRGBA:
			case renderer::PixelFormat::eBC4:
				return 0.5f;
			case renderer::PixelFormat::eBC2_RGBA:
			case renderer::PixelFormat::eBC2_SRGBA:
			case renderer::PixelFormat::eBC3_RGBA:
			case renderer::PixelFormat::eBC3_SRGBA:
			case renderer::PixelFormat::eBC5_RG:
				return 1.0f;
			case renderer::PixelFormat::eASTC_4x4_RGBA:
			case renderer::PixelFormat::eASTC_4x4_SRGBA:
				return 128.0f / 16.0f;
			case renderer::PixelFormat::eASTC_5x4_RGBA:
			case renderer::PixelFormat::eASTC_5x4_SRGBA:
				return 128.0f / 20.0f;
			case renderer::PixelFormat::eASTC_5x5_RGBA:
			case renderer::PixelFormat::eASTC_5x5_SRGBA:
				return 128.0f / 25.0f;
			case renderer::PixelFormat::eASTC_6x5_RGBA:
			case renderer::PixelFormat::eASTC_6x5_SRGBA:
				return 128.0f / 30.0f;
			case renderer::PixelFormat::eASTC_6x6_RGBA:
			case renderer::PixelFormat::eASTC_6x6_SRGBA:
				return 128.0f / 36.0f;
			case renderer::PixelFormat::eASTC_8x5_RGBA:
			case renderer::PixelFormat::eASTC_8x5_SRGBA:
				return 128.0f / 40.0f;
			case renderer::PixelFormat::eASTC_8x6_RGBA:
			case renderer::PixelFormat::eASTC_8x6_SRGBA:
				return 128.0f / 48.0f;
			case renderer::PixelFormat::eASTC_8x8_RGBA:
			case renderer::PixelFormat::eASTC_8x8_SRGBA:
				return 128.0f / 64.0f;
			case renderer::PixelFormat::eASTC_10x5_RGBA:
			case renderer::PixelFormat::eASTC_10x5_SRGBA:
				return 128.0f / 50.0f;
			case renderer::PixelFormat::eASTC_10x6_RGBA:
			case renderer::PixelFormat::eASTC_10x6_SRGBA:
				return 128.0f / 60.0f;
			case renderer::PixelFormat::eASTC_10x8_RGBA:
			case renderer::PixelFormat::eASTC_10x8_SRGBA:
				return 128.0f / 80.0f;
			case renderer::PixelFormat::eASTC_10x10_RGBA:
			case renderer::PixelFormat::eASTC_10x10_SRGBA:
				return 128.0f / 100.0f;
			case renderer::PixelFormat::eASTC_12x10_RGBA:
			case renderer::PixelFormat::eASTC_12x10_SRGBA:
				return 128.0f / 120.0f;
			case renderer::PixelFormat::eASTC_12x12_RGBA:
			case renderer::PixelFormat::eASTC_12x12_SRGBA:
				return 128.0f / 144.0f;
			case renderer::PixelFormat::eETC2_R8G8B8:
			case renderer::PixelFormat::eETC2_R8G8B8_SRGB:
			case renderer::PixelFormat::eETC2_R8G8B8A1:
			case renderer::PixelFormat::eETC2_R8G8B8A1_SRGB:
			case renderer::PixelFormat::eETC2_R8G8B8A8:
			case renderer::PixelFormat::eETC2_R8G8B8A8_SRGB:
			case renderer::PixelFormat::eEAC_R11U:
			case renderer::PixelFormat::eEAC_R11S:
			case renderer::PixelFormat::eEAC_R11G11U:
			case renderer::PixelFormat::eEAC_R11G11S:
				return 64.0f / 16.0f;

			default:
				return 1.0f;
			}
		}

		GLsizei getImageSize( renderer::PixelFormat format, uint32_t width )
		{
			return GLsizei( getCompressedFormatSize( format ) * width );
		}

		GLsizei getImageSize( renderer::PixelFormat format, uint32_t width, uint32_t height )
		{
			return GLsizei( getCompressedFormatSize( format ) * width * height );
		}

		GLsizei getImageSize( renderer::PixelFormat format, uint32_t width, uint32_t height, uint32_t depth )
		{
			return GLsizei( getCompressedFormatSize( format ) * width * height * depth );
		}

		enum GlTextureLevelParameter
		{
			GL_TEXTURE_WIDTH = 0x1000,
			GL_TEXTURE_HEIGHT = 0x1001,
			GL_TEXTURE_INTERNAL_FORMAT = 0x1003,
			GL_TEXTURE_COMPRESSED_IMAGE_SIZE = 0x86A0,
			GL_TEXTURE_COMPRESSED = 0x86A1,
			GL_TEXTURE_RED_TYPE = 0x8C10,
			GL_TEXTURE_GREEN_TYPE = 0x8C11,
			GL_TEXTURE_BLUE_TYPE = 0x8C12,
			GL_TEXTURE_ALPHA_TYPE = 0x8C13,
			GL_TEXTURE_DEPTH_TYPE = 0x8C16,
			GL_TEXTURE_BUFFER_OFFSET = 0x919D,
			GL_TEXTURE_BUFFER_SIZE = 0x919E,
		};
	}

	CopyBufferToImageCommand::CopyBufferToImageCommand( renderer::BufferImageCopyArray const & copyInfo
		, renderer::BufferBase const & src
		, renderer::Texture const & dst )
		: m_copyInfo{ copyInfo }
		, m_src{ static_cast< Buffer const & >( src ) }
		, m_dst{ static_cast< Texture const & >( dst ) }
		, m_format{ getInternal( m_dst.getFormat() ) }
		, m_type{ getType( m_dst.getFormat() ) }
		, m_copyTargets{ convert( m_dst.getType(), m_copyInfo ) }
		, m_views{ createViews( m_dst, m_copyInfo ) }
	{
	}

	void CopyBufferToImageCommand::apply()const
	{
		glLogCommand( "CopyBufferToImageCommand" );
		glLogCall( gl::BindBuffer, GL_BUFFER_TARGET_PIXEL_UNPACK, m_src.getBuffer() );

		for ( size_t i = 0; i < m_views.size(); ++i )
		{
			applyOne( m_copyInfo[i]
				, *m_views[i]
				, m_copyTargets[i] );
		}

		glLogCall( gl::BindBuffer, GL_BUFFER_TARGET_PIXEL_UNPACK, 0u );
	}

	CommandPtr CopyBufferToImageCommand::clone()const
	{
		return std::make_unique< CopyBufferToImageCommand >( *this );
	}

	void CopyBufferToImageCommand::applyOne( renderer::BufferImageCopy const & copyInfo
		, renderer::TextureView const & view
		, GlTextureType copyTarget )const
	{
		if ( copyTarget == GL_TEXTURE_3D )
		{
			glLogCall( gl::BindTexture, copyTarget, static_cast< Texture const & >( view.getTexture() ).getImage() );
		}
		else
		{
			glLogCall( gl::BindTexture, copyTarget, static_cast< TextureView const & >( view ).getImage() );
		}

		if ( renderer::isCompressedFormat( view.getFormat() ) )
		{
			switch ( copyTarget )
			{
			case GL_TEXTURE_1D:
				glLogCall( gl::CompressedTexSubImage1D
					, copyTarget
					, copyInfo.imageSubresource.mipLevel
					, copyInfo.imageOffset[0]
					, copyInfo.imageExtent[0]
					, m_format
					, copyInfo.levelSize
					, BufferOffset( copyInfo.bufferOffset ) );
				break;

			case GL_TEXTURE_2D:
			case GL_TEXTURE_CUBE_MAP:
				glLogCall( gl::CompressedTexSubImage2D
					, copyTarget
					, copyInfo.imageSubresource.mipLevel
					, copyInfo.imageOffset[0]
					, copyInfo.imageOffset[1]
					, copyInfo.imageExtent[0]
					, copyInfo.imageExtent[1]
					, m_format
					, copyInfo.levelSize
					, BufferOffset( copyInfo.bufferOffset ) );
				break;

			case GL_TEXTURE_3D:
				glLogCall( gl::CompressedTexSubImage3D
					, copyTarget
					, copyInfo.imageSubresource.mipLevel
					, copyInfo.imageOffset[0]
					, copyInfo.imageOffset[1]
					, copyInfo.imageOffset[2]
					, copyInfo.imageExtent[0]
					, copyInfo.imageExtent[1]
					, copyInfo.imageExtent[2]
					, m_format
					, copyInfo.levelSize
					, BufferOffset( copyInfo.bufferOffset ) );
				break;
			}
		}
		else
		{
			switch ( copyTarget )
			{
			case GL_TEXTURE_1D:
				glLogCall( gl::TexSubImage1D
					, copyTarget
					, copyInfo.imageSubresource.mipLevel
					, copyInfo.imageOffset[0]
					, copyInfo.imageExtent[0]
					, m_format
					, m_type
					, BufferOffset( copyInfo.bufferOffset ) );
				break;

			case GL_TEXTURE_2D:
			case GL_TEXTURE_CUBE_MAP:
				glLogCall( gl::TexSubImage2D
					, copyTarget
					, copyInfo.imageSubresource.mipLevel
					, copyInfo.imageOffset[0]
					, copyInfo.imageOffset[1]
					, copyInfo.imageExtent[0]
					, copyInfo.imageExtent[1]
					, m_format
					, m_type
					, BufferOffset( copyInfo.bufferOffset ) );
				break;

			case GL_TEXTURE_3D:
				glLogCall( gl::TexSubImage3D
					, copyTarget
					, copyInfo.imageSubresource.mipLevel
					, copyInfo.imageOffset[0]
					, copyInfo.imageOffset[1]
					, copyInfo.imageOffset[2]
					, copyInfo.imageExtent[0]
					, copyInfo.imageExtent[1]
					, copyInfo.imageExtent[2]
					, m_format
					, m_type
					, BufferOffset( copyInfo.bufferOffset ) );
				break;
			}
		}

		glLogCall( gl::BindTexture, copyTarget, 0u );
	}
}
