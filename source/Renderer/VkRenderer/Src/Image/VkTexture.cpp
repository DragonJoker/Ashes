#include "Image/VkTexture.hpp"

#include "Command/VkCommandBuffer.hpp"
#include "Command/VkQueue.hpp"
#include "Core/VkDevice.hpp"
#include "Core/VkRenderer.hpp"
#include "Sync/VkImageMemoryBarrier.hpp"
#include "Image/VkImageSubresourceRange.hpp"
#include "Miscellaneous/VkMemoryStorage.hpp"
#include "Command/VkQueue.hpp"
#include "Image/VkTextureView.hpp"

namespace vk_renderer
{
	namespace
	{
		renderer::ImageMemoryBarrier doConvert( Texture const & texture
			, VkImageMemoryBarrier const & barrier )
		{
			return renderer::ImageMemoryBarrier
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

		renderer::PixelFormat doFindSupportedFormat( Device const & device
			, const std::vector< renderer::PixelFormat > & candidates
			, VkImageTiling tiling
			, VkFormatFeatureFlags features
			, PhysicalDevice const & physicalDevice )
		{
			for ( renderer::PixelFormat format : candidates )
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

		renderer::PixelFormat doSelectFormat( Device const & device
			, renderer::PixelFormat format )
		{
			renderer::PixelFormat result;

			if ( renderer::isDepthOrStencilFormat( format ) )
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
		: renderer::Texture{ std::move( rhs ) }
		, m_device{ rhs.m_device }
		, m_image{ rhs.m_image }
		, m_owner{ rhs.m_owner }
	{
		rhs.m_image = VK_NULL_HANDLE;
	}

	Texture & Texture::operator=( Texture && rhs )
	{
		renderer::Texture::operator=( std::move( rhs ) );

		if ( &rhs != this )
		{
			m_image = rhs.m_image;
			m_owner = rhs.m_owner;
			rhs.m_image = VK_NULL_HANDLE;
		}

		return *this;
	}

	Texture::Texture( Device const & device
		, renderer::ImageCreateInfo const & createInfo
		, renderer::MemoryPropertyFlags memoryFlags )
		: renderer::Texture{ device
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
			getImageType( createInfo.imageType ),               // imageType
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

		if ( !checkError( res ) )
		{
			throw std::runtime_error{ "Image creation failed: " + getLastError() };
		}

		m_storage = std::make_unique< ImageStorage >( m_device
			, m_image
			, convert( memoryFlags ) );
		res = m_device.vkBindImageMemory( m_device
			, m_image
			, *m_storage
			, 0 );

		if ( !checkError( res ) )
		{
			throw std::runtime_error{ "Image storage binding failed: " + getLastError() };
		}
	}

	Texture::Texture( Device const & device
		, renderer::PixelFormat format
		, renderer::Extent2D const & dimensions
		, VkImage image )
		: renderer::Texture{ device
			, renderer::TextureType::e2D
			, doSelectFormat( m_device, format )
			, renderer::Extent3D{ dimensions.width, dimensions.height, 1u }
			, 1u 
			, 1u }
		, m_device{ device }
		, m_image{ image }
		, m_owner{ false }
	{
	}

	Texture::Texture( Device const & device
		, renderer::PixelFormat format
		, renderer::Extent2D const & dimensions
		, renderer::ImageUsageFlags usageFlags
		, renderer::ImageTiling tiling
		, renderer::MemoryPropertyFlags memoryFlags )
		: Texture{ device
			, {
				0u,
				renderer::TextureType::e2D,
				doSelectFormat( m_device, format ),
				renderer::Extent3D{ dimensions.width, dimensions.height, 1u },
				1u,
				1u,
				renderer::SampleCountFlag::e1,
				tiling,
				usageFlags,
				renderer::SharingMode::eExclusive,
				{},
				renderer::ImageLayout::eUndefined
			}
			, memoryFlags }
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

	renderer::Texture::Mapped Texture::lock( uint32_t offset
		, uint32_t size
		, VkMemoryMapFlags flags )const
	{
		renderer::Texture::Mapped mapped{};
		VkImageSubresource subResource{};
		subResource.aspectMask = getImageAspectFlags( getFormat() );
		VkSubresourceLayout subResourceLayout;
		m_device.vkGetImageSubresourceLayout( m_device, m_image, &subResource, &subResourceLayout );

		mapped.data = m_storage->lock( offset
			, size
			, flags );

		if ( mapped.data )
		{
			mapped.arrayPitch = subResourceLayout.arrayPitch;
			mapped.depthPitch = subResourceLayout.depthPitch;
			mapped.rowPitch = subResourceLayout.rowPitch;
			mapped.size = subResourceLayout.size;
			mapped.data += subResourceLayout.offset;
		}

		return mapped;
	}

	void Texture::flush( uint32_t offset
		, uint32_t size )const
	{
		m_storage->flush( offset, size );
	}

	void Texture::unlock()const
	{
		m_storage->unlock();
	}

	renderer::TextureViewPtr Texture::createView( renderer::ImageViewCreateInfo const & createInfo )const
	{
		return std::make_shared< TextureView >( m_device
			, *this
			, createInfo );
	}

	void Texture::generateMipmaps()const
	{
		auto const width = int32_t( getDimensions().width );
		auto const height = int32_t( getDimensions().height );
		auto commandBuffer = m_device.getGraphicsCommandPool().createCommandBuffer();
		auto & vkCommandBuffer = static_cast< CommandBuffer const & >( *commandBuffer );

		if ( commandBuffer->begin( renderer::CommandBufferUsageFlag::eOneTimeSubmit ) )
		{
			TextureView srcView{ m_device
				, *this
				, {
					getType(),
					getFormat(),
					renderer::ComponentMapping{},
					{
						renderer::getAspectMask( getFormat() ),
						0,
						1u,
						0u,
						1u
					}
				} };
			vkCommandBuffer.memoryBarrier( convert( renderer::PipelineStageFlag::eFragmentShader )
				, convert( renderer::PipelineStageFlag::eTopOfPipe )
				, srcView.makeTransferSource( renderer::ImageLayout::eUndefined, 0u ) );

			for ( uint32_t i = 1u; i < getMipmapLevels(); ++i )
			{
				TextureView dstView{ m_device
					, *this
					, {
						getType(),
						getFormat(),
						renderer::ComponentMapping{},
						{
							renderer::getAspectMask( getFormat() ),
							0,
							1u,
							0u,
							1u
						}
					} };
				vkCommandBuffer.memoryBarrier( convert( renderer::PipelineStageFlag::eFragmentShader )
					, convert( renderer::PipelineStageFlag::eTopOfPipe )
					, dstView.makeTransferDestination( renderer::ImageLayout::eUndefined, 0u ) );

				int32_t const mipWidth = width >> i;
				int32_t const mipHeight = height >> i;

				VkImageBlit imageBlit = {};
				imageBlit.srcSubresource.aspectMask = getImageAspectFlags( getFormat() );
				imageBlit.srcSubresource.baseArrayLayer = 0;
				imageBlit.srcSubresource.layerCount = 1;
				imageBlit.srcSubresource.mipLevel = 0;
				imageBlit.srcOffsets[0] = { 0, 0, 0 };
				imageBlit.srcOffsets[1] = { width, height, 1 };

				imageBlit.dstSubresource.aspectMask = getImageAspectFlags( getFormat() );
				imageBlit.dstSubresource.baseArrayLayer = 0;
				imageBlit.dstSubresource.layerCount = 1;
				imageBlit.dstSubresource.mipLevel = i;
				imageBlit.dstOffsets[0] = { 0, 0, 0 };
				imageBlit.dstOffsets[1] = { mipWidth, mipHeight, 1 };

				m_device.vkCmdBlitImage( vkCommandBuffer
					, m_image
					, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL
					, m_image
					, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
					, 1
					, &imageBlit
					, VK_FILTER_LINEAR );
			}

			commandBuffer->end();
			m_device.getGraphicsQueue().submit( *commandBuffer, nullptr );
			m_device.waitIdle();
		}
	}
}
