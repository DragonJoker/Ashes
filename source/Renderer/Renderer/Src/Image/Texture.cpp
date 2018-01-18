/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#include "Image/Texture.hpp"

#include "Buffer/StagingBuffer.hpp"
#include "Image/TextureView.hpp"

namespace renderer
{
	Texture::Texture( Device const & device )
		: m_device{ device }
	{
	}

	void Texture::setImage( PixelFormat format
		, uint32_t size
		, ImageUsageFlags usageFlags
		, ImageTiling tiling )
	{
		m_format = format;
		m_type = TextureType::e1D;
		m_size = UIVec3{ size, 0u, 0u };
		m_layerCount = 0u;
		m_samples = SampleCountFlag::e1;
		doSetImage1D( usageFlags
			, tiling
			, renderer::MemoryPropertyFlag::eDeviceLocal );
		m_view = m_device.createTextureView( *this
			, m_format
			, 0u
			, 1u
			, 0u
			, 1u );
	}

	void Texture::setImage( PixelFormat format
		, UIVec2 const & size
		, ImageUsageFlags usageFlags
		, ImageTiling tiling
		, SampleCountFlag samples )
	{
		m_format = format;
		m_type = TextureType::e2D;
		m_size = UIVec3{ size[0], size[1], 0u };
		m_layerCount = 0;
		m_samples = samples;
		doSetImage2D( usageFlags
			, tiling
			, renderer::MemoryPropertyFlag::eDeviceLocal );
		m_view = m_device.createTextureView( *this
			, m_format
			, 0u
			, 1u
			, 0u
			, 1u );
	}

	void Texture::setImage( PixelFormat format
		, UIVec3 const & size
		, ImageUsageFlags usageFlags
		, ImageTiling tiling
		, SampleCountFlag samples )
	{
		m_format = format;
		m_type = TextureType::e3D;
		m_size = size;
		m_layerCount = 0;
		m_samples = samples;
		doSetImage3D( usageFlags
			, tiling
			, renderer::MemoryPropertyFlag::eDeviceLocal );
		m_view = m_device.createTextureView( *this
			, m_format
			, 0u
			, 1u
			, 0u
			, 1u );
	}

	void Texture::setImage( PixelFormat format
		, uint32_t size
		, uint32_t layerCount
		, ImageUsageFlags usageFlags
		, ImageTiling tiling )
	{
		m_format = format;
		m_type = TextureType::e1D;
		m_size = UIVec3{ size, 0u, 0u };
		m_layerCount = layerCount;
		m_samples = SampleCountFlag::e1;
		doSetImage1D( usageFlags
			, tiling
			, renderer::MemoryPropertyFlag::eDeviceLocal );
		m_view = m_device.createTextureView( *this
			, m_format
			, 0u
			, 1u
			, 0u
			, 1u );
	}

	void Texture::setImage( PixelFormat format
		, UIVec2 const & size
		, uint32_t layerCount
		, ImageUsageFlags usageFlags
		, ImageTiling tiling )
	{
		m_format = format;
		m_type = TextureType::e2D;
		m_size = UIVec3{ size[0], size[1], 0u };
		m_layerCount = layerCount;
		m_samples = SampleCountFlag::e1;
		doSetImage2D( usageFlags
			, tiling
			, renderer::MemoryPropertyFlag::eDeviceLocal );
		m_view = m_device.createTextureView( *this
			, m_format
			, 0u
			, 1u
			, 0u
			, 1u );
	}
}
