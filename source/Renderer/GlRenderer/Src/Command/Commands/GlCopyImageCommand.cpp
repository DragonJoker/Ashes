/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "GlCopyImageCommand.hpp"

#include "Buffer/GlBuffer.hpp"
#include "Image/GlTexture.hpp"

#include <Image/ImageSubresourceRange.hpp>
#include <Image/TextureView.hpp>

namespace gl_renderer
{
	CopyImageCommand::CopyImageCommand( renderer::BufferBase const & src
		, renderer::Texture const & dst )
		: m_src{ static_cast< Buffer const & >( src ) }
		, m_dst{ static_cast< Texture const & >( dst ) }
		, m_format{ getFormat( m_dst.getFormat() ) }
		, m_type{ getType( m_dst.getFormat() ) }
		, m_target{ convert( m_dst.getType() ) }
	{
	}

	void CopyImageCommand::apply()const
	{
		auto & range = m_dst.getView().getSubResourceRange();
		glBindTexture( m_target, m_dst.getImage() );
		glBindBuffer( GL_PIXEL_UNPACK_BUFFER, m_src.getBuffer() );

		switch ( m_target )
		{
		case GL_TEXTURE_1D:
			glTexSubImage1D( m_target
				, range.getBaseMipLevel()
				, 0
				, m_dst.getDimensions()[0]
				, m_format
				, m_type
				, nullptr );
			break;

		case GL_TEXTURE_2D:
			glTexSubImage2D( m_target
				, range.getBaseMipLevel()
				, 0
				, 0
				, m_dst.getDimensions()[0]
				, m_dst.getDimensions()[1]
				, m_format
				, m_type
				, nullptr );
			break;

		case GL_TEXTURE_3D:
			glTexSubImage3D( m_target
				, range.getBaseMipLevel()
				, 0
				, 0
				, 0
				, m_dst.getDimensions()[0]
				, m_dst.getDimensions()[1]
				, m_dst.getDimensions()[2]
				, m_format
				, m_type
				, nullptr );
			break;
		}

		glBindBuffer( GL_PIXEL_UNPACK_BUFFER, 0u );
		glBindTexture( m_target, 0u );
		m_dst.generateMipmaps();
	}

	CommandPtr CopyImageCommand::clone()const
	{
		return std::make_unique< CopyImageCommand >( *this );
	}
}
