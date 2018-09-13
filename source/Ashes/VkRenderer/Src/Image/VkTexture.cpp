#include "Image/VkTexture.hpp"

#include "Command/VkCommandBuffer.hpp"
#include "Command/VkQueue.hpp"
#include "Core/VkDevice.hpp"
#include "Core/VkPhysicalDevice.hpp"
#include "Core/VkRenderer.hpp"
#include "Sync/VkImageMemoryBarrier.hpp"
#include "Image/VkImageSubresourceRange.hpp"
#include "Miscellaneous/VkDeviceMemory.hpp"
#include "Command/VkQueue.hpp"
#include "Image/VkTextureView.hpp"

namespace vk_renderer
{
	namespace
	{
		ashes::ImageMemoryBarrier doConvert( Texture const & texture
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
				device.getRenderer().vkGetPhysicalDeviceFormatProperties( physicalDevice
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
	}

	Texture::Texture( Texture && rhs )
		: ashes::Texture{ std::move( rhs ) }
		, m_device{ rhs.m_device }
		, m_image{ rhs.m_image }
		, m_owner{ rhs.m_owner }
	{
		rhs.m_image = VK_NULL_HANDLE;
	}

	Texture & Texture::operator=( Texture && rhs )
	{
		ashes::Texture::operator=( std::move( rhs ) );

		if ( &rhs != this )
		{
			m_image = rhs.m_image;
			m_owner = rhs.m_owner;
			rhs.m_image = VK_NULL_HANDLE;
		}

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
		, m_image{}
		, m_owner{ true }
	{
		VkImageCreateInfo vkcreateInfo
		{
			VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
			nullptr,
			convert( createInfo.flags ),                        // flags
			convert( createInfo.imageType ),                    // imageType
			convert( createInfo.format ),                       // format
			convert( createInfo.extent ),                       // extent
			createInfo.mipLevels,                               // mipLevels
			createInfo.arrayLayers,                             // arrayLayers
			convert( createInfo.samples ),                      // samples
			convert( createInfo.tiling ),                       // tiling
			convert( createInfo.usage ),                        // usage
			convert( createInfo.sharingMode ),                  // sharingMode
			uint32_t( createInfo.queueFamilyIndices.size() ),   // queueFamilyIndexCount
			createInfo.queueFamilyIndices.empty()               // pQueueFamilyIndices
				? nullptr
				: createInfo.queueFamilyIndices.data(),
			convert( createInfo.initialLayout )                 // initialLayout
		};
		DEBUG_DUMP( vkcreateInfo );
		auto res = m_device.vkCreateImage( m_device
			, &vkcreateInfo
			, nullptr
			, &m_image );
		checkError( res, "Image creation" );
	}

	Texture::Texture( Device const & device
		, ashes::Format format
		, ashes::Extent2D const & dimensions
		, VkImage image )
		: ashes::Texture{ device
			, 0u
			, ashes::TextureType::e2D
			, doSelectFormat( device, format )
			, ashes::Extent3D{ dimensions.width, dimensions.height, 1u }
			, 1u 
			, 1u }
		, m_device{ device }
		, m_image{ image }
		, m_owner{ false }
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

	Texture::~Texture()
	{
		if ( m_owner )
		{
			m_device.vkDestroyImage( m_device
				, m_image
				, nullptr );
		}
	}

	ashes::MemoryRequirements Texture::getMemoryRequirements()const
	{
		return m_device.getImageMemoryRequirements( m_image );
	}

	ashes::TextureViewPtr Texture::createView( ashes::ImageViewCreateInfo const & createInfo )const
	{
		return std::make_unique< TextureView >( m_device
			, *this
			, createInfo );
	}

	void Texture::doBindMemory()
	{
		auto res = m_device.vkBindImageMemory( m_device
			, m_image
			, static_cast< DeviceMemory const & >( *m_storage )
			, 0 );
		checkError( res, "Image storage binding" );
	}
}
