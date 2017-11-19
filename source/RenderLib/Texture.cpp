#include "Texture.h"

#include <GlLib/GlTexture.h>
#include <GlLib/GlSampler.h>
#include <GlLib/OpenGL.h>

namespace render
{
	void Texture::image( gl::PixelFormat format
		, gl::IVec2 const & size
		, ByteArray const & data
		, gl::WrapMode wrapS
		, gl::WrapMode wrapT
		, gl::MinFilter minFilter
		, gl::MagFilter magFilter )
	{
		m_format = format;
		m_size = size;
		m_glTexture = std::make_unique < gl::Texture >( m_format
			, m_size
			, data );
		m_glSampler = gl::OpenGL::createSampler( wrapS
			, wrapT
			, minFilter
			, magFilter );
	}

	void Texture::generateMipmaps()const noexcept
	{
		m_glTexture->bind( 0 );
		m_glTexture->generateMipmaps();
		m_glTexture->unbind( 0 );
	}

	void Texture::bind( uint32_t unit )const noexcept
	{
		assert( m_glTexture && m_glSampler );
		m_glTexture->bind( unit );
		m_glSampler->bind( unit );
	}

	void Texture::unbind( uint32_t unit )const noexcept
	{
		assert( m_glTexture && m_glSampler );
		m_glSampler->unbind( unit );
		m_glTexture->unbind( unit );
	}
}
