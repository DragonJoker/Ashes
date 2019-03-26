#include "Image/TestImage.hpp"

#include "Command/TestCommandBuffer.hpp"
#include "Command/TestQueue.hpp"
#include "Core/TestDevice.hpp"
#include "Core/TestPhysicalDevice.hpp"
#include "Core/TestInstance.hpp"
#include "Miscellaneous/TestDeviceMemory.hpp"
#include "Command/TestQueue.hpp"
#include "Image/TestImageView.hpp"

#include <Ashes/Miscellaneous/Extent2D.hpp>
#include <Ashes/Sync/ImageMemoryBarrier.hpp>
#include <Ashes/Image/ImageSubresourceRange.hpp>

namespace test_renderer
{
	Image::Image( Image && rhs )
		: ashes::Image{ std::move( rhs ) }
		, m_device{ rhs.m_device }
	{
	}

	Image & Image::operator=( Image && rhs )
	{
		ashes::Image::operator=( std::move( rhs ) );
		return *this;
	}

	Image::Image( Device const & device
		, ashes::ImageCreateInfo const & createInfo )
		: ashes::Image{ device
			, createInfo.flags
			, createInfo.imageType
			, createInfo.format
			, createInfo.extent
			, createInfo.mipLevels
			, createInfo.arrayLayers }
		, m_device{ device }
	{
	}

	Image::Image( Device const & device
		, ashes::Format format
		, ashes::Extent2D const & dimensions )
		: ashes::Image{ device
			, 0u
			, ashes::ImageType::e2D
			, format
			, ashes::Extent3D{ dimensions.width, dimensions.height, 1u }
			, 1u 
			, 1u }
		, m_device{ device }
	{
	}

	Image::Image( Device const & device
		, ashes::Format format
		, ashes::Extent2D const & dimensions
		, ashes::ImageUsageFlags usageFlags
		, ashes::ImageTiling tiling
		, ashes::MemoryPropertyFlags memoryFlags )
		: Image{ device
			, {
				0u,
				ashes::ImageType::e2D,
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

	Image::~Image()
	{
	}

	ashes::MemoryRequirements Image::getMemoryRequirements()const
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
		result.memoryTypeBits = ~result.memoryTypeBits;
		return result;
	}

	ashes::ImageViewPtr Image::createView( ashes::ImageViewCreateInfo const & createInfo )const
	{
		return std::make_shared< ImageView >( m_device
			, *this
			, createInfo );
	}

	void Image::doBindMemory()
	{
	}
}
