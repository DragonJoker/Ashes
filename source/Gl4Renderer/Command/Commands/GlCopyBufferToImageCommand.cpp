/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlCopyBufferToImageCommand.hpp"

#include "Buffer/GlBuffer.hpp"
#include "Image/GlImage.hpp"
#include "Image/GlImageView.hpp"
#include "Miscellaneous/GlCallLogger.hpp"

#include "ashesgl4_api.hpp"

#define getBufferOffset( n ) ( ( uint8_t * )nullptr + ( n ) )

namespace ashes::gl4
{
	enum GlPackAlignment
	{
		GL_UNPACK_ALIGNMENT = 0x0CF5,
		GL_PACK_ALIGNMENT = 0x0D05,
	};

	CopyBufferToImageCommand::CopyBufferToImageCommand( VkDevice device
		, VkBufferImageCopy copyInfo
		, VkBuffer src
		, VkImage dst )
		: CommandBase{ device }
		, m_copyInfo{ copyInfo }
		, m_src{ static_cast< VkBuffer >( src ) }
		, m_dst{ static_cast< VkImage >( dst ) }
		, m_internal{ getInternalFormat( get( m_dst )->getFormat() ) }
		, m_format{ getFormat( m_internal ) }
		, m_type{ getType( m_internal ) }
		, m_copyTarget{ convert( get( m_dst )->getType(), get( m_dst )->getArrayLayers() ) }
	{
	}

	void CopyBufferToImageCommand::apply( ContextLock const & context )const
	{
		glLogCommand( "CopyBufferToImageCommand" );
		glLogCall( context
			, glBindTexture
			, m_copyTarget
			, get( m_dst )->getInternal() );
		glLogCall( context
			, glPixelStorei
			, GL_UNPACK_ALIGNMENT
			, 1 );
		glLogCall( context
			, glBindBuffer
			, GL_BUFFER_TARGET_PIXEL_UNPACK
			, get( m_src )->getInternal() );

		if ( isCompressedFormat( get( m_dst )->getFormat() ) )
		{
			switch ( m_copyTarget )
			{
			case GL_TEXTURE_1D:
				glLogCall( context
					, glCompressedTexSubImage1D
					, m_copyTarget
					, m_copyInfo.imageSubresource.mipLevel
					, m_copyInfo.imageOffset.x
					, m_copyInfo.imageExtent.width
					, m_internal
					, getSize( m_copyInfo.imageExtent, get( m_dst )->getFormat() )
					, getBufferOffset( m_copyInfo.bufferOffset ) );
				break;

			case GL_TEXTURE_2D:
				glLogCall( context
					, glCompressedTexSubImage2D
					, m_copyTarget
					, m_copyInfo.imageSubresource.mipLevel
					, m_copyInfo.imageOffset.x
					, m_copyInfo.imageOffset.y
					, m_copyInfo.imageExtent.width
					, m_copyInfo.imageExtent.height
					, m_internal
					, getSize( m_copyInfo.imageExtent, get( m_dst )->getFormat() )
					, getBufferOffset( m_copyInfo.bufferOffset ) );
				break;

			case GL_TEXTURE_3D:
				glLogCall( context
					, glCompressedTexSubImage3D
					, m_copyTarget
					, m_copyInfo.imageSubresource.mipLevel
					, m_copyInfo.imageOffset.x
					, m_copyInfo.imageOffset.y
					, m_copyInfo.imageOffset.z
					, m_copyInfo.imageExtent.width
					, m_copyInfo.imageExtent.height
					, m_copyInfo.imageExtent.depth
					, m_internal
					, getSize( m_copyInfo.imageExtent, get( m_dst )->getFormat() )
					, getBufferOffset( m_copyInfo.bufferOffset ) );

			case GL_TEXTURE_1D_ARRAY:
				glLogCall( context
					, glCompressedTexSubImage2D
					, m_copyTarget
					, m_copyInfo.imageSubresource.mipLevel
					, m_copyInfo.imageOffset.x
					, m_copyInfo.imageSubresource.baseArrayLayer
					, m_copyInfo.imageExtent.width
					, m_copyInfo.imageSubresource.layerCount
					, m_internal
					, getSize( m_copyInfo.imageExtent, get( m_dst )->getFormat() )
					, getBufferOffset( m_copyInfo.bufferOffset ) );
				break;

			case GL_TEXTURE_2D_ARRAY:
				glLogCall( context
					, glCompressedTexSubImage3D
					, m_copyTarget
					, m_copyInfo.imageSubresource.mipLevel
					, m_copyInfo.imageOffset.x
					, m_copyInfo.imageOffset.y
					, m_copyInfo.imageSubresource.baseArrayLayer
					, m_copyInfo.imageExtent.width
					, m_copyInfo.imageExtent.height
					, m_copyInfo.imageSubresource.layerCount
					, m_internal
					, getSize( m_copyInfo.imageExtent, get( m_dst )->getFormat() )
					, getBufferOffset( m_copyInfo.bufferOffset ) );
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
					, m_copyInfo.imageSubresource.mipLevel
					, m_copyInfo.imageOffset.x
					, m_copyInfo.imageExtent.width
					, m_format
					, m_type
					, getBufferOffset( m_copyInfo.bufferOffset ) );
				break;

			case GL_TEXTURE_2D:
				glLogCall( context
					, glTexSubImage2D
					, m_copyTarget
					, m_copyInfo.imageSubresource.mipLevel
					, m_copyInfo.imageOffset.x
					, m_copyInfo.imageOffset.y
					, m_copyInfo.imageExtent.width
					, m_copyInfo.imageExtent.height
					, m_format
					, m_type
					, getBufferOffset( m_copyInfo.bufferOffset ) );
				break;

			case GL_TEXTURE_3D:
				glLogCall( context
					, glTexSubImage3D
					, m_copyTarget
					, m_copyInfo.imageSubresource.mipLevel
					, m_copyInfo.imageOffset.x
					, m_copyInfo.imageOffset.y
					, m_copyInfo.imageOffset.z
					, m_copyInfo.imageExtent.width
					, m_copyInfo.imageExtent.height
					, m_copyInfo.imageExtent.depth
					, m_format
					, m_type
					, getBufferOffset( m_copyInfo.bufferOffset ) );
				break;

			case GL_TEXTURE_1D_ARRAY:
				glLogCall( context
					, glTexSubImage2D
					, m_copyTarget
					, m_copyInfo.imageSubresource.mipLevel
					, m_copyInfo.imageOffset.x
					, m_copyInfo.imageSubresource.baseArrayLayer
					, m_copyInfo.imageExtent.width
					, m_copyInfo.imageSubresource.layerCount
					, m_format
					, m_type
					, getBufferOffset( m_copyInfo.bufferOffset ) );
				break;

			case GL_TEXTURE_2D_ARRAY:
				glLogCall( context
					, glTexSubImage3D
					, m_copyTarget
					, m_copyInfo.imageSubresource.mipLevel
					, m_copyInfo.imageOffset.x
					, m_copyInfo.imageOffset.y
					, m_copyInfo.imageSubresource.baseArrayLayer
					, m_copyInfo.imageExtent.width
					, m_copyInfo.imageExtent.height
					, m_copyInfo.imageSubresource.layerCount
					, m_format
					, m_type
					, getBufferOffset( m_copyInfo.bufferOffset ) );
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

	CommandPtr CopyBufferToImageCommand::clone()const
	{
		return std::make_unique< CopyBufferToImageCommand >( *this );
	}
}
