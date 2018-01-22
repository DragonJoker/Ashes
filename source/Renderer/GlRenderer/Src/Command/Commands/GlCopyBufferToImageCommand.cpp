/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "GlCopyBufferToImageCommand.hpp"

#include "Buffer/GlBuffer.hpp"
#include "Image/GlTexture.hpp"

#include <Image/ImageSubresourceRange.hpp>
#include <Image/TextureView.hpp>

namespace gl_renderer
{
	CopyBufferToImageCommand::CopyBufferToImageCommand( renderer::BufferImageCopy const & copyInfo
		, renderer::BufferBase const & src
		, renderer::Texture const & dst )
		: m_copyInfo{ copyInfo }
		, m_src{ static_cast< Buffer const & >( src ) }
		, m_dst{ static_cast< Texture const & >( dst ) }
		, m_format{ getFormat( m_dst.getFormat() ) }
		, m_type{ getType( m_dst.getFormat() ) }
		, m_target{ convert( m_dst.getType() ) }
	{
	}

	void CopyBufferToImageCommand::apply()const
	{
		glLogCall( glBindTexture, m_target, m_dst.getImage() );
		glLogCall( glBindBuffer, GL_PIXEL_UNPACK_BUFFER, m_src.getBuffer() );

		switch ( m_target )
		{
		case GL_TEXTURE_1D:
			glLogCall( glTexSubImage1D
				, m_target
				, m_copyInfo.imageSubresource.mipLevel
				, m_copyInfo.imageOffset[0]
				, m_copyInfo.imageExtent[0]
				, m_format
				, m_type
				, nullptr );
			break;

		case GL_TEXTURE_2D:
			glLogCall( glTexSubImage2D
				, m_target
				, m_copyInfo.imageSubresource.mipLevel
				, m_copyInfo.imageOffset[0]
				, m_copyInfo.imageOffset[1]
				, m_copyInfo.imageExtent[0]
				, m_copyInfo.imageExtent[1]
				, m_format
				, m_type
				, nullptr );
			break;

		case GL_TEXTURE_3D:
			glLogCall( glTexSubImage3D
				, m_target
				, m_copyInfo.imageSubresource.mipLevel
				, m_copyInfo.imageOffset[0]
				, m_copyInfo.imageOffset[1]
				, m_copyInfo.imageOffset[2]
				, m_copyInfo.imageExtent[0]
				, m_copyInfo.imageExtent[1]
				, m_copyInfo.imageExtent[2]
				, m_format
				, m_type
				, nullptr );
			break;
		}

		glLogCall( glBindBuffer, GL_PIXEL_UNPACK_BUFFER, 0u );
		glLogCall( glBindTexture, m_target, 0u );
		m_dst.generateMipmaps();
	}

	CommandPtr CopyBufferToImageCommand::clone()const
	{
		return std::make_unique< CopyBufferToImageCommand >( *this );
	}
}
