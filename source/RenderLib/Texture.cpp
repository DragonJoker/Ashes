#include "Texture.h"

#include "RenderingResources.hpp"

namespace render
{
	Texture::Texture( RenderingResources const & resources )
		: m_resources{ resources }
	{
	}

	void Texture::image( renderer::PixelFormat format
		, renderer::IVec2 const & size
		, ByteArray const & data
		, vk::WrapMode wrapS
		, vk::WrapMode wrapT
		, vk::Filter minFilter
		, vk::Filter magFilter )
	{
		m_format = format;
		m_size = size;
		m_texture = m_resources.getDevice().createImage( m_format
			, m_size.x
			, m_size.y
			, vk::MemoryPropertyFlag::eDeviceLocal );
		m_sampler = m_resources.getDevice().createSampler( minFilter
			, magFilter
			, vk::MipmapMode::eNearest
			, wrapS
			, wrapT
			, vk::WrapMode::eRepeat );
		m_resources.copyTextureData( data, *m_texture );
	}

	void Texture::generateMipmaps()const noexcept
	{
		//m_texture->bind( 0 );
		//m_texture->generateMipmaps();
		//m_texture->unbind( 0 );
	}

	void Texture::bind( uint32_t unit )const noexcept
	{
		assert( m_texture && m_sampler );
		m_texture->bind( unit );
		m_sampler->bind( unit );
	}

	void Texture::unbind( uint32_t unit )const noexcept
	{
		assert( m_texture && m_sampler );
		m_sampler->unbind( unit );
		m_texture->unbind( unit );
	}
}
