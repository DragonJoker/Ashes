/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "GlCopyImageToBufferCommand.hpp"

#include "Buffer/GlBuffer.hpp"
#include "Image/GlTexture.hpp"

#include <Image/ImageSubresourceRange.hpp>
#include <Image/TextureView.hpp>

namespace gl_renderer
{
	CopyImageToBufferCommand::CopyImageToBufferCommand( renderer::BufferImageCopy const & copyInfo
		, renderer::Texture const & src
		, renderer::BufferBase const & dst )
		: m_copyInfo{ copyInfo }
		, m_src{ static_cast< Texture const & >( src ) }
		, m_dst{ static_cast< Buffer const & >( dst ) }
		, m_format{ getFormat( m_src.getFormat() ) }
		, m_type{ getType( m_src.getFormat() ) }
		, m_target{ convert( m_src.getType() ) }
	{
	}

	void CopyImageToBufferCommand::apply()const
	{
		glLogCall( glBindTexture, m_target, m_src.getImage() );
		glLogCall( glBindBuffer, GL_PIXEL_PACK_BUFFER, m_dst.getBuffer() );
		glLogCall( glGetTexImage
			, m_target
			, m_copyInfo.imageSubresource.mipLevel
			, m_format
			, m_type
			, nullptr );
		glLogCall( glBindBuffer, GL_PIXEL_PACK_BUFFER, 0u );
		glLogCall( glBindTexture, m_target, 0u );
	}

	CommandPtr CopyImageToBufferCommand::clone()const
	{
		return std::make_unique< CopyImageToBufferCommand >( *this );
	}
}
