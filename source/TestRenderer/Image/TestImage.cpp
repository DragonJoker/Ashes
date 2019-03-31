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
		: ashes::Image{ device, createInfo }
		, m_device{ device }
	{
	}

	Image::Image( Device const & device
		, ashes::Format format
		, ashes::Extent2D const & dimensions )
		: ashes::Image{ device
			, {
				0u,
				ashes::ImageType::e2D,
				format,
				ashes::Extent3D{ dimensions.width, dimensions.height, 1u },
				1u,
				1u,
				ashes::SampleCountFlag::e1,
				ashes::ImageTiling::eOptimal,
				( isDepthOrStencilFormat( format )
					? ashes::ImageUsageFlag::eDepthStencilAttachment
					: ashes::ImageUsageFlag::eColourAttachment ),
				ashes::SharingMode::eExclusive,
				{},
				ashes::ImageLayout::eUndefined
			} }
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
		result.size = ashes::getSize( getDimensions(), getFormat() );
		result.type = ashes::ResourceType::eImage;
		auto extent = ashes::getMinimalExtent3D( getFormat() );
		result.alignment = ashes::getSize( extent, getFormat() );
		result.memoryTypeBits = ashes::MemoryPropertyFlag::eDeviceLocal
			| ( ( checkFlag( m_createInfo.usage, ashes::ImageUsageFlag::eTransferDst )
				&& checkFlag( m_createInfo.usage, ashes::ImageUsageFlag::eTransferSrc ) )
				? ashes::MemoryPropertyFlag::eHostVisible
				: ashes::MemoryPropertyFlag::eDeviceLocal );
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
