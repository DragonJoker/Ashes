#include "Texture.h"

#include <Renderer/Texture.hpp>
#include <Renderer/Sampler.hpp>

namespace render
{
	Texture::Texture( renderer::Device const & device
		, renderer::WrapMode wrapS
		, renderer::WrapMode wrapT
		, renderer::Filter minFilter
		, renderer::Filter magFilter )
		: m_texture{ device }
		, m_sampler{ device
			, wrapS
			, wrapT
			, minFilter
			, magFilter }
	{
	}

	void Texture::image( utils::PixelFormat format
		, utils::IVec2 const & size
		, ByteArray const & data
		, renderer::RenderingResources const & resources )
	{
		m_format = format;
		m_size = size;
		m_texture.setImage( m_format
			, m_size
			, data
			, resources );
	}
}
