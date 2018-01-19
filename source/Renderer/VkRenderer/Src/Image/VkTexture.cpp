#include "Image/VkTexture.hpp"

#include "Command/VkCommandBuffer.hpp"
#include "Core/VkDevice.hpp"
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
	}

	Texture::Texture( Device const & device )
		: renderer::Texture{ device }
		, m_device{ device }
		, m_image{}
		, m_owner{ true }
		, m_currentLayout{ renderer::ImageLayout::eUndefined }
		, m_currentAccessMask{ 0 }
	{
	}

	Texture::Texture( Device const & device
		, renderer::PixelFormat format
		, renderer::UIVec2 const & dimensions
		, VkImage image )
		: renderer::Texture{ device }
		, m_device{ device }
		, m_image{ image }
		, m_owner{ false }
		, m_currentLayout{ renderer::ImageLayout::ePresentSrc }
		, m_currentAccessMask{ renderer::AccessFlag::eMemoryRead }
	{
		m_format = format;
		m_type = renderer::TextureType::e2D;
		m_size = renderer::UIVec3{ dimensions[0], dimensions[1], 0u };
		m_layerCount = 0u;
		m_samples = renderer::SampleCountFlag::e1;
		m_view = std::make_unique< TextureView >( m_device
			, *this
			, m_format
			, 0u
			, 1u
			, 0u
			, 1u );
	}

	Texture::Texture( Device const & device
		, renderer::PixelFormat format
		, renderer::UIVec2 const & dimensions
		, renderer::ImageUsageFlags usageFlags
		, renderer::ImageTiling tiling
		, renderer::MemoryPropertyFlags memoryFlags )
		: renderer::Texture{ device }
		, m_device{ device }
		, m_owner{ true }
		, m_currentLayout{ renderer::ImageLayout::eUndefined }
		, m_currentAccessMask{ 0 }
	{
		m_format = format;
		m_type = renderer::TextureType::e2D;
		m_size = renderer::UIVec3{ dimensions[0], dimensions[1], 0u };
		m_layerCount = 0u;
		m_samples = renderer::SampleCountFlag::e1;
		doSetImage2D( usageFlags
			, tiling
			, memoryFlags );
	}

	renderer::Texture::Mapped Texture::lock( uint32_t offset
		, uint32_t size
		, VkMemoryMapFlags flags )const
	{
		renderer::Texture::Mapped mapped{};
		VkImageSubresource subResource{};
		subResource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		VkSubresourceLayout subResourceLayout;
		GetImageSubresourceLayout( m_device, m_image, &subResource, &subResourceLayout );

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

	void Texture::unlock( uint32_t size
		, bool modified )const
	{
		m_storage->unlock( size
			, modified );
	}

	void Texture::doSetImage1D( renderer::ImageUsageFlags usageFlags
		, renderer::ImageTiling tiling
		, renderer::MemoryPropertyFlags memoryFlags )
	{
		assert( m_owner );
		VkImageCreateInfo createInfo
		{
			VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
			nullptr,
			0,                                                    // flags
			VK_IMAGE_TYPE_1D,                                     // imageType
			convert( m_format ),                                  // format
			{                                                     // extent
				uint32_t( m_size[0] ),                                // width
				1u,                                                   // height
				1u                                                    // depth
			},
			1,                                                    // mipLevels
			m_layerCount ? m_layerCount : 1u,                     // arrayLayers
			convert( m_samples ),                                 // samples
			convert( tiling ),                                    // tiling
			convert( usageFlags ),                                // usage
			VK_SHARING_MODE_EXCLUSIVE,                            // sharingMode
			0,                                                    // queueFamilyIndexCount
			nullptr,                                              // pQueueFamilyIndices
			VK_IMAGE_LAYOUT_UNDEFINED                             // initialLayout
		};
		DEBUG_DUMP( createInfo );
		auto res = CreateImage( m_device
			, &createInfo
			, nullptr
			, &m_image );

		if ( !checkError( res ) )
		{
			throw std::runtime_error{ "Image creation failed: " + getLastError() };
		}

		m_storage = std::make_unique< ImageStorage >( m_device
			, m_image
			, convert( memoryFlags ) );
		res = BindImageMemory( m_device
			, m_image
			, *m_storage
			, 0 );

		if ( !checkError( res ) )
		{
			throw std::runtime_error{ "Image storage binding failed: " + getLastError() };
		}
	}

	void Texture::doSetImage2D( renderer::ImageUsageFlags usageFlags
		, renderer::ImageTiling tiling
		, renderer::MemoryPropertyFlags memoryFlags )
	{
		assert( m_owner );
		VkImageCreateInfo createInfo
		{
			VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
			nullptr,
			0,                                                    // flags
			VK_IMAGE_TYPE_2D,                                     // imageType
			convert( m_format ),                                  // format
			{                                                     // extent
				uint32_t( m_size[0] ),                                // width
				uint32_t( m_size[1] ),                                // height
				1                                                     // depth
			},
			1,                                                    // mipLevels
			m_layerCount ? m_layerCount : 1u,                     // arrayLayers
			convert( m_samples ),                                 // samples
			convert( tiling ),                                    // tiling
			convert( usageFlags ),                                // usage
			VK_SHARING_MODE_EXCLUSIVE,                            // sharingMode
			0,                                                    // queueFamilyIndexCount
			nullptr,                                              // pQueueFamilyIndices
			VK_IMAGE_LAYOUT_UNDEFINED                             // initialLayout
		};
		DEBUG_DUMP( createInfo );
		auto res = CreateImage( m_device
			, &createInfo
			, nullptr
			, &m_image );

		if ( !checkError( res ) )
		{
			throw std::runtime_error{ "Image creation failed: " + getLastError() };
		}

		m_storage = std::make_unique< ImageStorage >( m_device
			, m_image
			, convert( memoryFlags ) );
		res = BindImageMemory( m_device
			, m_image
			, *m_storage
			, 0 );

		if ( !checkError( res ) )
		{
			throw std::runtime_error{ "Image storage binding failed: " + getLastError() };
		}
	}

	void Texture::doSetImage3D( renderer::ImageUsageFlags usageFlags
		, renderer::ImageTiling tiling
		, renderer::MemoryPropertyFlags memoryFlags )
	{
		assert( m_owner );
		VkImageCreateInfo createInfo
		{
			VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
			nullptr,
			0,                                                    // flags
			VK_IMAGE_TYPE_3D,                                     // imageType
			convert( m_format ),                                  // format
			{                                                     // extent
				uint32_t( m_size[0] ),                                // width
				uint32_t( m_size[1] ),                                // height
				uint32_t( m_size[2] ),                                // depth
			},
			1u,                                                   // mipLevels
			1u,                                                   // arrayLayers
			convert( m_samples ),                                 // samples
			convert( tiling ),                                    // tiling
			convert( usageFlags ),                                // usage
			VK_SHARING_MODE_EXCLUSIVE,                            // sharingMode
			0,                                                    // queueFamilyIndexCount
			nullptr,                                              // pQueueFamilyIndices
			VK_IMAGE_LAYOUT_UNDEFINED                             // initialLayout
		};
		DEBUG_DUMP( createInfo );
		auto res = CreateImage( m_device
			, &createInfo
			, nullptr
			, &m_image );

		if ( !checkError( res ) )
		{
			throw std::runtime_error{ "Image creation failed: " + getLastError() };
		}

		m_storage = std::make_unique< ImageStorage >( m_device
			, m_image
			, convert( memoryFlags ) );
		res = BindImageMemory( m_device
			, m_image
			, *m_storage
			, 0 );

		if ( !checkError( res ) )
		{
			throw std::runtime_error{ "Image storage binding failed: " + getLastError() };
		}
	}

	void Texture::generateMipmaps()const
	{
		//m_texture->bind( 0 );
		//m_texture->generateMipmaps();
		//m_texture->unbind( 0 );
	}

	renderer::ImageMemoryBarrier Texture::makeGeneralLayout( renderer::AccessFlags accessFlags )const
	{
		return doMakeLayoutTransition( renderer::ImageLayout::eGeneral
			, VK_QUEUE_FAMILY_IGNORED
			, accessFlags );
	}

	renderer::ImageMemoryBarrier Texture::makeTransferDestination()const
	{
		return doMakeLayoutTransition( renderer::ImageLayout::eTransferDstOptimal
			, VK_QUEUE_FAMILY_IGNORED
			, renderer::AccessFlag::eTransferWrite );
	}

	renderer::ImageMemoryBarrier Texture::makeTransferSource()const
	{
		return doMakeLayoutTransition( renderer::ImageLayout::eTransferSrcOptimal
			, VK_QUEUE_FAMILY_IGNORED
			, renderer::AccessFlag::eTransferRead );
	}

	renderer::ImageMemoryBarrier Texture::makeShaderInputResource()const
	{
		return doMakeLayoutTransition( renderer::ImageLayout::eShaderReadOnlyOptimal
			, VK_QUEUE_FAMILY_IGNORED
			, renderer::AccessFlag::eShaderRead );
	}

	renderer::ImageMemoryBarrier Texture::makeDepthStencilReadOnly()const
	{
		return doMakeLayoutTransition( renderer::ImageLayout::eDepthStencilReadOnlyOptimal
			, m_device.getGraphicsQueue().getFamilyIndex()
			, renderer::AccessFlag::eShaderRead );
	}

	renderer::ImageMemoryBarrier Texture::makeColourAttachment()const
	{
		return doMakeLayoutTransition( renderer::ImageLayout::eColourAttachmentOptimal
			, m_device.getGraphicsQueue().getFamilyIndex()
			, renderer::AccessFlag::eColourAttachmentWrite );
	}

	renderer::ImageMemoryBarrier Texture::makeDepthStencilAttachment()const
	{
		return doMakeLayoutTransition( renderer::ImageLayout::eDepthStencilAttachmentOptimal
			, m_device.getGraphicsQueue().getFamilyIndex()
			, renderer::AccessFlag::eColourAttachmentWrite );
	}

	renderer::ImageMemoryBarrier Texture::makePresentSource()const
	{
		return doMakeLayoutTransition( renderer::ImageLayout::ePresentSrc
			, m_device.getPresentQueue().getFamilyIndex()
			, renderer::AccessFlag::eMemoryRead );
	}

	renderer::ImageMemoryBarrier Texture::doMakeLayoutTransition( renderer::ImageLayout layout
		, uint32_t queueFamily
		, renderer::AccessFlags dstAccessMask )const
	{
		// On fait passer le layout de l'image � un autre layout, via une barri�re.
		renderer::ImageMemoryBarrier transitionBarrier
		{
			m_currentAccessMask,                     // srcAccessMask
			dstAccessMask,                           // dstAccessMask
			m_currentLayout,                         // oldLayout
			layout,                                  // newLayout
			queueFamily == VK_QUEUE_FAMILY_IGNORED   // srcQueueFamilyIndex
				? VK_QUEUE_FAMILY_IGNORED
				: m_currentQueueFamily,
			queueFamily,                             // dstQueueFamilyIndex
			*this,                                   // image
			m_view->getSubResourceRange()            // subresourceRange
		};
		DEBUG_DUMP( convert( transitionBarrier ) );
		m_currentAccessMask = dstAccessMask;
		m_currentLayout = layout;
		m_currentQueueFamily = queueFamily == VK_QUEUE_FAMILY_IGNORED
			? m_currentQueueFamily
			: queueFamily;
		return transitionBarrier;
	}
}
