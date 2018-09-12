#include "Image/D3D11Texture.hpp"

#include "Command/D3D11CommandBuffer.hpp"
#include "Command/D3D11Queue.hpp"
#include "Core/D3D11Device.hpp"
#include "Core/D3D11PhysicalDevice.hpp"
#include "Core/D3D11Renderer.hpp"
#include "Miscellaneous/D3D11DeviceMemory.hpp"
#include "Command/D3D11Queue.hpp"
#include "Image/D3D11TextureView.hpp"

#include <Miscellaneous/Extent2D.hpp>
#include <Sync/ImageMemoryBarrier.hpp>
#include <Image/ImageSubresourceRange.hpp>

namespace d3d11_renderer
{
	Texture::Texture( Texture && rhs )
		: renderer::Texture{ std::move( rhs ) }
		, m_device{ rhs.m_device }
		, m_image{ rhs.m_image }
		, m_createInfo{ std::move( rhs.m_createInfo ) }
	{
		rhs.m_image.tex1D = nullptr;
	}

	Texture & Texture::operator=( Texture && rhs )
	{
		renderer::Texture::operator=( std::move( rhs ) );
		m_image = rhs.m_image;
		rhs.m_image.tex1D = nullptr;
		return *this;
	}

	Texture::Texture( Device const & device
		, renderer::ImageCreateInfo const & createInfo )
		: renderer::Texture{ device
			, createInfo.flags
			, createInfo.imageType
			, createInfo.format
			, createInfo.extent
			, createInfo.mipLevels
			, createInfo.arrayLayers }
		, m_device{ device }
		, m_createInfo{ createInfo }
		, m_image{ nullptr }
	{
	}

	Texture::Texture( Device const & device
		, renderer::Format format
		, renderer::Extent2D const & dimensions
		, ID3D11Texture2D * image )
		: renderer::Texture{ device
			, 0u
			, renderer::TextureType::e2D
			, format
			, renderer::Extent3D{ dimensions.width, dimensions.height, 1u }
			, 1u 
			, 1u }
		, m_device{ device }
		, m_image{ nullptr }
		, m_createInfo
		{
			0u,
			renderer::TextureType::e2D,
			format,
			renderer::Extent3D{ dimensions.width, dimensions.height, 1u },
			1u,
			1u,
			renderer::SampleCountFlag::e1,
			renderer::ImageTiling::eOptimal,
			( isDepthOrStencilFormat( format )
				? renderer::ImageUsageFlag::eDepthStencilAttachment
				: renderer::ImageUsageFlag::eColourAttachment ),
			renderer::SharingMode::eExclusive,
			{},
			renderer::ImageLayout::eUndefined
		}
	{
		m_image.tex2D = image;
	}

	Texture::Texture( Device const & device
		, renderer::Format format
		, renderer::Extent2D const & dimensions
		, renderer::ImageUsageFlags usageFlags
		, renderer::ImageTiling tiling
		, renderer::MemoryPropertyFlags memoryFlags )
		: Texture{ device
			, {
				0u,
				renderer::TextureType::e2D,
				format,
				renderer::Extent3D{ dimensions.width, dimensions.height, 1u },
				1u,
				1u,
				renderer::SampleCountFlag::e1,
				tiling,
				usageFlags,
				renderer::SharingMode::eExclusive,
				{},
				renderer::ImageLayout::eUndefined
			} }
	{
	}

	Texture::~Texture()
	{
		safeRelease( m_image.tex1D );
	}

	renderer::MemoryRequirements Texture::getMemoryRequirements()const
	{
		renderer::MemoryRequirements result{};

		if ( !renderer::isCompressedFormat( getFormat() ) )
		{
			result.size = getDimensions().width
				* getDimensions().height
				* getDimensions().depth
				* getLayerCount()
				* renderer::getSize( getFormat() );
		}

		result.type = renderer::ResourceType::eImage;
		result.alignment = 1u;
		result.memoryTypeBits = 0xFFFFFFFF;
		return result;
	}

	renderer::TextureViewPtr Texture::createView( renderer::ImageViewCreateInfo const & createInfo )const
	{
		return std::make_unique< TextureView >( m_device
			, *this
			, createInfo );
	}

	void Texture::doBindMemory()
	{
		switch ( getType() )
		{
		case renderer::TextureType::e1D:
			m_image.tex1D = static_cast< DeviceMemory & >( *m_storage ).bindToTexture1D( m_createInfo );
			break;

		case renderer::TextureType::e2D:
			m_image.tex2D = static_cast< DeviceMemory & >( *m_storage ).bindToTexture2D( m_createInfo );
			break;

		case renderer::TextureType::e3D:
			m_image.tex3D = static_cast< DeviceMemory & >( *m_storage ).bindToTexture3D( m_createInfo );
			break;
		}
	}
}
