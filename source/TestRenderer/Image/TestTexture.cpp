#include "Image/TestTexture.hpp"

#include "Command/TestCommandBuffer.hpp"
#include "Command/TestQueue.hpp"
#include "Core/TestDevice.hpp"
#include "Core/TestPhysicalDevice.hpp"
#include "Core/TestInstance.hpp"
#include "Miscellaneous/TestDeviceMemory.hpp"
#include "Command/TestQueue.hpp"
#include "Image/TestTextureView.hpp"

#include <Ashes/Miscellaneous/Extent2D.hpp>
#include <Ashes/Sync/ImageMemoryBarrier.hpp>
#include <Ashes/Image/ImageSubresourceRange.hpp>

namespace test_renderer
{
	Texture::Texture( Texture && rhs )
		: ashes::Texture{ std::move( rhs ) }
		, m_device{ rhs.m_device }
	{
	}

	Texture & Texture::operator=( Texture && rhs )
	{
		ashes::Texture::operator=( std::move( rhs ) );
		return *this;
	}

	Texture::Texture( Device const & device
		, ashes::ImageCreateInfo const & createInfo )
		: ashes::Texture{ device
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
		, ashes::Format format
		, ashes::Extent2D const & dimensions )
		: ashes::Texture{ device
			, 0u
			, ashes::TextureType::e2D
			, format
			, ashes::Extent3D{ dimensions.width, dimensions.height, 1u }
			, 1u 
			, 1u }
		, m_device{ device }
	{
	}

	Texture::Texture( Device const & device
		, ashes::Format format
		, ashes::Extent2D const & dimensions
		, ashes::ImageUsageFlags usageFlags
		, ashes::ImageTiling tiling
		, ashes::MemoryPropertyFlags memoryFlags )
		: Texture{ device
			, {
				0u,
				ashes::TextureType::e2D,
				format,
				ashes::Extent3D{ dimensions.width, dimensions.height, 1u },
				1u,
				1u,
				ashes::SampleCountFlag::e1,
				tiling,
				usageFlags,
				ashes::SharingMode::eExclusive,
				{},
				ashes::ImageLayout::eUndefined
			} }
	{
	}

	Texture::~Texture()
	{
	}

	ashes::MemoryRequirements Texture::getMemoryRequirements()const
	{
		ashes::MemoryRequirements result{};

		if ( !ashes::isCompressedFormat( getFormat() ) )
		{
			result.size = getDimensions().width
				* getDimensions().height
				* getDimensions().depth
				* getLayerCount()
				* ashes::getSize( getFormat() );
		}

		result.type = ashes::ResourceType::eImage;
		result.alignment = 1u;
		result.memoryTypeBits = 0xFFFFFFFF;
		return result;
	}

	ashes::TextureViewPtr Texture::createView( ashes::ImageViewCreateInfo const & createInfo )const
	{
		return std::make_unique< TextureView >( m_device
			, *this
			, createInfo );
	}

	void Texture::doBindMemory()
	{
	}
}
