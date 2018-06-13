#include "Image/TestTexture.hpp"

#include "Command/TestCommandBuffer.hpp"
#include "Command/TestQueue.hpp"
#include "Core/TestDevice.hpp"
#include "Core/TestPhysicalDevice.hpp"
#include "Core/TestRenderer.hpp"
#include "Miscellaneous/TestDeviceMemory.hpp"
#include "Command/TestQueue.hpp"
#include "Image/TestTextureView.hpp"

#include <Miscellaneous/Extent2D.hpp>
#include <Sync/ImageMemoryBarrier.hpp>
#include <Image/ImageSubresourceRange.hpp>

namespace test_renderer
{
	Texture::Texture( Texture && rhs )
		: renderer::Texture{ std::move( rhs ) }
		, m_device{ rhs.m_device }
	{
	}

	Texture & Texture::operator=( Texture && rhs )
	{
		renderer::Texture::operator=( std::move( rhs ) );
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
	{
	}

	Texture::Texture( Device const & device
		, renderer::Format format
		, renderer::Extent2D const & dimensions )
		: renderer::Texture{ device
			, 0u
			, renderer::TextureType::e2D
			, format
			, renderer::Extent3D{ dimensions.width, dimensions.height, 1u }
			, 1u 
			, 1u }
		, m_device{ device }
	{
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
	}
}
