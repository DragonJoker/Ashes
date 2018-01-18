/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "GlCopyImageCommand.hpp"

#include "GlRenderer/GlBuffer.hpp"
#include "GlRenderer/GlTexture.hpp"

#include <Renderer/Image/ImageSubresourceRange.hpp>
#include <Renderer/Image/TextureView.hpp>

namespace gl_renderer
{
	CopyImageCommand::CopyImageCommand( renderer::BufferBase const & src
		, renderer::Texture const & dst )
		: m_src{ static_cast< Buffer const & >( src ) }
		, m_dst{ static_cast< Texture const & >( dst ) }
		, m_format{ getFormat( m_dst.getFormat() ) }
		, m_type{ getType( m_dst.getFormat() ) }
	{
	}

	void CopyImageCommand::apply()const
	{
		auto & range = m_dst.getView().getSubResourceRange();
		glBindTexture( GL_TEXTURE_2D, m_dst.getImage() );
		glBindBuffer( GL_PIXEL_UNPACK_BUFFER, m_src.getBuffer() );
		glTexSubImage2D( GL_TEXTURE_2D
			, range.getBaseMipLevel()
			, 0
			, 0
			, m_dst.getDimensions().x
			, m_dst.getDimensions().y
			, m_format
			, m_type
			, nullptr );
		glBindBuffer( GL_PIXEL_UNPACK_BUFFER, 0u );
		glBindTexture( GL_TEXTURE_2D, 0u );
		m_dst.generateMipmaps();
	}

	CommandPtr CopyImageCommand::clone()const
	{
		return std::make_unique< CopyImageCommand >( *this );
	}
}
