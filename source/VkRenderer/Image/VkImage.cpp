#include "Image/VkImage.hpp"

#include "Command/VkCommandBuffer.hpp"
#include "Command/VkQueue.hpp"
#include "Core/VkDevice.hpp"
#include "Core/VkPhysicalDevice.hpp"
#include "Core/VkInstance.hpp"
#include "Sync/VkImageMemoryBarrier.hpp"
#include "Image/VkImageSubresourceRange.hpp"
#include "Miscellaneous/VkDeviceMemory.hpp"
#include "Command/VkQueue.hpp"
#include "Image/VkImageView.hpp"

namespace vk_renderer
{
	namespace
	{
		ashes::ImageMemoryBarrier doConvert( Image const & texture
			, VkImageMemoryBarrier const & barrier )
		{
			return ashes::ImageMemoryBarrier
			{
				convertAccessFlags( barrier.srcAccessMask ),
				convertAccessFlags( barrier.dstAccessMask ),
				convertImageLayout( barrier.oldLayout ),
				convertImageLayout( barrier.newLayout ),
				barrier.srcQueueFamilyIndex,
				barrier.dstQueueFamilyIndex,
				texture,
				convert( barrier.subresourceRange )
			};
		}

		ashes::Format doFindSupportedFormat( Device const & device
			, const std::vector< ashes::Format > & candidates
			, VkImageTiling tiling
			, VkFormatFeatureFlags features
			, PhysicalDevice const & physicalDevice )
		{
			for ( ashes::Format format : candidates )
			{
				VkFormatProperties props;
				device.getInstance().vkGetPhysicalDeviceFormatProperties( physicalDevice
					, convert( format )
					, &props );

				if ( tiling == VK_IMAGE_TILING_LINEAR && ( props.linearTilingFeatures & features ) == features )
				{
					return format;
				}

				if ( tiling == VK_IMAGE_TILING_OPTIMAL && ( props.optimalTilingFeatures & features ) == features )
				{
					return format;
				}
			}

			throw std::runtime_error( "failed to find supported format!" );
		}

		ashes::Format doSelectFormat( Device const & device
			, ashes::Format format )
		{
			ashes::Format result;

			if ( ashes::isDepthOrStencilFormat( format ) )
			{
				result = doFindSupportedFormat( device
					, { format }
					, VK_IMAGE_TILING_OPTIMAL
					, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
					, static_cast< PhysicalDevice const & >( device.getPhysicalDevice() ) );
			}
			else
			{
				result = format;
			}

			return result;
		}

		VkImageCreateInfo convert( ashes::ImageCreateInfo const & value )
		{
			return
			{
				VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
				nullptr,
				vk_renderer::convert( value.flags ),           // flags
				vk_renderer::convert( value.imageType ),       // imageType
				vk_renderer::convert( value.format ),          // format
				vk_renderer::convert( value.extent ),          // extent
				value.mipLevels,                               // mipLevels
				value.arrayLayers,                             // arrayLayers
				vk_renderer::convert( value.samples ),         // samples
				vk_renderer::convert( value.tiling ),          // tiling
				vk_renderer::convert( value.usage ),           // usage
				vk_renderer::convert( value.sharingMode ),     // sharingMode
				uint32_t( value.queueFamilyIndices.size() ),   // queueFamilyIndexCount
				value.queueFamilyIndices.empty()               // pQueueFamilyIndices
					? nullptr
					: value.queueFamilyIndices.data(),
				vk_renderer::convert( value.initialLayout )    // initialLayout
			};
		}
	}

	Image::Image( Image && rhs )
		: ashes::Image{ std::move( rhs ) }
		, m_device{ rhs.m_device }
		, m_image{ rhs.m_image }
		, m_owner{ rhs.m_owner }
		, m_vkCreateInfo{ rhs.m_vkCreateInfo }
	{
		rhs.m_image = VK_NULL_HANDLE;
	}

	Image & Image::operator=( Image && rhs )
	{
		ashes::Image::operator=( std::move( rhs ) );

		if ( &rhs != this )
		{
			m_image = rhs.m_image;
			m_owner = rhs.m_owner;
			m_vkCreateInfo = rhs.m_vkCreateInfo;
			rhs.m_image = VK_NULL_HANDLE;
		}

		return *this;
	}

	Image::Image( Device const & device
		, ashes::ImageCreateInfo const & createInfo )
		: ashes::Image{ device, createInfo }
		, m_device{ device }
		, m_image{}
		, m_owner{ true }
		, m_vkCreateInfo{ convert( m_createInfo ) }
	{
		DEBUG_DUMP( m_vkCreateInfo );
		auto res = m_device.vkCreateImage( m_device
			, &m_vkCreateInfo
			, nullptr
			, &m_image );
		checkError( res, "Image creation" );
	}

	Image::Image( Device const & device
		, ashes::Format format
		, ashes::Extent2D const & dimensions
		, VkImage image )
		: ashes::Image{ device
			, {
				0u,
				ashes::ImageType::e2D,
				doSelectFormat( device, format ),
				ashes::Extent3D{ dimensions.width, dimensions.height, 1u },
				1u,
				1u,
				ashes::SampleCountFlag::e1,
				ashes::ImageTiling::eOptimal,
				ashes::ImageUsageFlag::eNone,
				ashes::SharingMode::eExclusive,
				{},
				ashes::ImageLayout::eUndefined
			} }
		, m_device{ device }
		, m_image{ image }
		, m_owner{ false }
		, m_vkCreateInfo{ convert( m_createInfo ) }
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
				doSelectFormat( device, format ),
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
		if ( m_owner )
		{
			m_device.vkDestroyImage( m_device
				, m_image
				, nullptr );
		}
	}

	ashes::MemoryRequirements Image::getMemoryRequirements()const
	{
		VkMemoryRequirements requirements;
		m_device.vkGetImageMemoryRequirements( m_device
			, m_image
			, &requirements );
		ashes::MemoryRequirements result = convert( requirements );
		result.type = ashes::ResourceType::eImage;
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
		auto res = m_device.vkBindImageMemory( m_device
			, m_image
			, static_cast< DeviceMemory const & >( *m_storage )
			, 0 );
		checkError( res, "Image storage binding" );
	}
}
