#include "Texture.h"

#include <Renderer/Texture.hpp>
#include <Renderer/Sampler.hpp>

namespace render
{
	Texture::Texture( renderer::Device const & device
		, renderer::WrapMode wrapS
		, renderer::WrapMode wrapT
		, renderer::WrapMode wrapR
		, renderer::Filter minFilter
		, renderer::Filter magFilter )
		: m_texture{ device }
		, m_sampler{ device
			, wrapS
			, wrapT
			, wrapR
			, minFilter
			, magFilter }
	{
	}

	void Texture::image( utils::PixelFormat format
		, utils::IVec2 const & size
		, ByteArray const & data
		, renderer::StagingBuffer const & stagingBuffer
		, renderer::CommandBuffer const & commandBuffer )
	{
		m_format = format;
		m_size = size;
		m_texture.setImage( m_format
			, m_size
			, data
			, stagingBuffer
			, commandBuffer
			, renderer::ImageUsageFlag::eTransferDst | renderer::ImageUsageFlag::eSampled
			, m_format == utils::PixelFormat::eR8G8B8
				? renderer::ImageTiling::eLinear
				: renderer::ImageTiling::eOptimal );
	}
}
