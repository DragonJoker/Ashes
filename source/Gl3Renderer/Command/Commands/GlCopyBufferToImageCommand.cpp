/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlCopyBufferToImageCommand.hpp"

#include "Buffer/GlBuffer.hpp"
#include "Image/GlTexture.hpp"
#include "Image/GlTextureView.hpp"

#include <Ashes/Image/ImageSubresourceRange.hpp>

#define BufferOffset( n ) ( ( uint8_t * )nullptr + ( n ) )

namespace gl_renderer
{
	enum GlPackAlignment
	{
		GL_UNPACK_ALIGNMENT = 0x0CF5,
		GL_PACK_ALIGNMENT = 0x0D05,
	};

	CopyBufferToImageCommand::CopyBufferToImageCommand( Device const & device
		, ashes::BufferImageCopyArray const & copyInfo
		, ashes::BufferBase const & src
		, ashes::Texture const & dst )
		: CommandBase{ device }
		, m_copyInfo{ copyInfo }
		, m_src{ static_cast< Buffer const & >( src ) }
		, m_dst{ static_cast< Texture const & >( dst ) }
		, m_internal{ getInternal( m_dst.getFormat() ) }
		, m_format{ getFormat( m_internal ) }
		, m_type{ getType( m_internal ) }
		, m_copyTarget{ convert( m_dst.getType(), m_dst.getLayerCount(), m_dst.getFlags() ) }
	{
	}

	void CopyBufferToImageCommand::apply( ContextLock const & context )const
	{
		glLogCommand( "CopyBufferToImageCommand" );

		for (const auto & copyInfo : m_copyInfo)
		{
			applyOne( context, copyInfo );
		}
	}

	CommandPtr CopyBufferToImageCommand::clone()const
	{
		return std::make_unique< CopyBufferToImageCommand >( *this );
	}

	void CopyBufferToImageCommand::applyOne( ContextLock const & context
		, ashes::BufferImageCopy const & copyInfo )const
	{
		glLogCall( context
			, glBindTexture
			, m_copyTarget
			, m_dst.getImage() );
		glLogCall( context
			, glPixelStorei
			, GL_UNPACK_ALIGNMENT
			, 1 );
		glLogCall( context
			, glBindBuffer
			, GL_BUFFER_TARGET_PIXEL_UNPACK
			, m_src.getBuffer() );

		if ( ashes::isCompressedFormat( m_dst.getFormat() ) )
		{
			switch ( m_copyTarget )
			{
			case GL_TEXTURE_1D:
				glLogCall( context
					, glCompressedTexSubImage1D
					, m_copyTarget
					, copyInfo.imageSubresource.mipLevel
					, copyInfo.imageOffset.x
					, copyInfo.imageExtent.width
					, m_internal
					, getSize( copyInfo.imageExtent, m_dst.getFormat() )
					, BufferOffset( copyInfo.bufferOffset ) );
				break;

			case GL_TEXTURE_2D:
				glLogCall( context
					, glCompressedTexSubImage2D
					, m_copyTarget
					, copyInfo.imageSubresource.mipLevel
					, copyInfo.imageOffset.x
					, copyInfo.imageOffset.y
					, copyInfo.imageExtent.width
					, copyInfo.imageExtent.height
					, m_internal
					, getSize( copyInfo.imageExtent, m_dst.getFormat() )
					, BufferOffset( copyInfo.bufferOffset ) );
				break;

			case GL_TEXTURE_3D:
				glLogCall( context
					, glCompressedTexSubImage3D
					, m_copyTarget
					, copyInfo.imageSubresource.mipLevel
					, copyInfo.imageOffset.x
					, copyInfo.imageOffset.y
					, copyInfo.imageOffset.z
					, copyInfo.imageExtent.width
					, copyInfo.imageExtent.height
					, copyInfo.imageExtent.depth
					, m_internal
					, getSize( copyInfo.imageExtent, m_dst.getFormat() )
					, BufferOffset( copyInfo.bufferOffset ) );

			case GL_TEXTURE_1D_ARRAY:
				glLogCall( context
					, glCompressedTexSubImage2D
					, m_copyTarget
					, copyInfo.imageSubresource.mipLevel
					, copyInfo.imageOffset.x
					, copyInfo.imageSubresource.baseArrayLayer
					, copyInfo.imageExtent.width
					, copyInfo.imageSubresource.layerCount
					, m_internal
					, getSize( copyInfo.imageExtent, m_dst.getFormat() )
					, BufferOffset( copyInfo.bufferOffset ) );
				break;

			case GL_TEXTURE_2D_ARRAY:
				glLogCall( context
					, glCompressedTexSubImage3D
					, m_copyTarget
					, copyInfo.imageSubresource.mipLevel
					, copyInfo.imageOffset.x
					, copyInfo.imageOffset.y
					, copyInfo.imageSubresource.baseArrayLayer
					, copyInfo.imageExtent.width
					, copyInfo.imageExtent.height
					, copyInfo.imageSubresource.layerCount
					, m_internal
					, getSize( copyInfo.imageExtent, m_dst.getFormat() )
					, BufferOffset( copyInfo.bufferOffset ) );
				break;

			case GL_TEXTURE_CUBE:
				glLogCall( context
					, glCompressedTexSubImage2D
					, GL_TEXTURE_CUBE_POSITIVE_X + copyInfo.imageSubresource.baseArrayLayer
					, copyInfo.imageSubresource.mipLevel
					, copyInfo.imageOffset.x
					, copyInfo.imageOffset.y
					, copyInfo.imageExtent.width
					, copyInfo.imageExtent.height
					, m_internal
					, getSize( copyInfo.imageExtent, m_dst.getFormat() )
					, BufferOffset( copyInfo.bufferOffset ) );
				break;

			case GL_TEXTURE_CUBE_ARRAY:
				glLogCall( context
					, glCompressedTexSubImage3D
					, GL_TEXTURE_CUBE_POSITIVE_X + ( copyInfo.imageSubresource.baseArrayLayer % 6u )
					, copyInfo.imageSubresource.mipLevel
					, copyInfo.imageOffset.x
					, copyInfo.imageOffset.y
					, copyInfo.imageSubresource.baseArrayLayer / 6u
					, copyInfo.imageExtent.width
					, copyInfo.imageExtent.height
					, copyInfo.imageSubresource.layerCount / 6u
					, m_internal
					, getSize( copyInfo.imageExtent, m_dst.getFormat() )
					, BufferOffset( copyInfo.bufferOffset ) );
				break;
			}
		}
		else
		{
			switch ( m_copyTarget )
			{
			case GL_TEXTURE_1D:
				glLogCall( context
					, glTexSubImage1D
					, m_copyTarget
					, copyInfo.imageSubresource.mipLevel
					, copyInfo.imageOffset.x
					, copyInfo.imageExtent.width
					, m_format
					, m_type
					, BufferOffset( copyInfo.bufferOffset ) );
				break;

			case GL_TEXTURE_2D:
				glLogCall( context
					, glTexSubImage2D
					, m_copyTarget
					, copyInfo.imageSubresource.mipLevel
					, copyInfo.imageOffset.x
					, copyInfo.imageOffset.y
					, copyInfo.imageExtent.width
					, copyInfo.imageExtent.height
					, m_format
					, m_type
					, BufferOffset( copyInfo.bufferOffset ) );
				break;

			case GL_TEXTURE_3D:
				glLogCall( context
					, glTexSubImage3D
					, m_copyTarget
					, copyInfo.imageSubresource.mipLevel
					, copyInfo.imageOffset.x
					, copyInfo.imageOffset.y
					, copyInfo.imageOffset.z
					, copyInfo.imageExtent.width
					, copyInfo.imageExtent.height
					, copyInfo.imageExtent.depth
					, m_format
					, m_type
					, BufferOffset( copyInfo.bufferOffset ) );
				break;

			case GL_TEXTURE_1D_ARRAY:
				glLogCall( context
					, glTexSubImage2D
					, m_copyTarget
					, copyInfo.imageSubresource.mipLevel
					, copyInfo.imageOffset.x
					, copyInfo.imageSubresource.baseArrayLayer
					, copyInfo.imageExtent.width
					, copyInfo.imageSubresource.layerCount
					, m_format
					, m_type
					, BufferOffset( copyInfo.bufferOffset ) );
				break;

			case GL_TEXTURE_2D_ARRAY:
				glLogCall( context
					, glTexSubImage3D
					, m_copyTarget
					, copyInfo.imageSubresource.mipLevel
					, copyInfo.imageOffset.x
					, copyInfo.imageOffset.y
					, copyInfo.imageSubresource.baseArrayLayer
					, copyInfo.imageExtent.width
					, copyInfo.imageExtent.height
					, copyInfo.imageSubresource.layerCount
					, m_format
					, m_type
					, BufferOffset( copyInfo.bufferOffset ) );
				break;

			case GL_TEXTURE_CUBE:
				glLogCall( context
					, glTexSubImage2D
					, GL_TEXTURE_CUBE_POSITIVE_X + copyInfo.imageSubresource.baseArrayLayer
					, copyInfo.imageSubresource.mipLevel
					, copyInfo.imageOffset.x
					, copyInfo.imageOffset.y
					, copyInfo.imageExtent.width
					, copyInfo.imageExtent.height
					, m_format
					, m_type
					, BufferOffset( copyInfo.bufferOffset ) );
				break;

			case GL_TEXTURE_CUBE_ARRAY:
				glLogCall( context
					, glTexSubImage3D
					, GL_TEXTURE_CUBE_POSITIVE_X + ( copyInfo.imageSubresource.baseArrayLayer % 6u )
					, copyInfo.imageSubresource.mipLevel
					, copyInfo.imageOffset.x
					, copyInfo.imageOffset.y
					, copyInfo.imageSubresource.baseArrayLayer / 6u
					, copyInfo.imageExtent.width
					, copyInfo.imageExtent.height
					, copyInfo.imageSubresource.layerCount / 6u
					, m_format
					, m_type
					, BufferOffset( copyInfo.bufferOffset ) );
				break;
			}
		}

		glLogCall( context
			, glBindBuffer
			, GL_BUFFER_TARGET_PIXEL_UNPACK
			, 0u );
		glLogCall( context
			, glBindTexture
			, m_copyTarget
			, 0u );
	}
}
