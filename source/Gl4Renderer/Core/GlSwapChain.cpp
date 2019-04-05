#include "Core/GlSwapChain.hpp"

#include "Core/GlDevice.hpp"
#include "Image/GlImage.hpp"
#include "Image/GlImageView.hpp"
#include "Miscellaneous/GlCallLogger.hpp"
#include "RenderPass/GlFrameBuffer.hpp"

#include <vulkan/vulkan.h>
#include "ashesgl4_api.hpp"

namespace ashes
{
	inline uint32_t deepCopy( uint32_t const & rhs )
	{
		return rhs;
	}
}

namespace ashes::gl4
{
	namespace
	{
		VkImage createImage( VkDevice device
			, VkFormat format
			, VkExtent2D dimensions )
		{
			VkImage result;
			allocate( result
				, nullptr
				, device
				, format
				, std::move( dimensions ) );
			auto requirements = get( result )->getMemoryRequirements();
			uint32_t deduced = deduceMemoryType( requirements.memoryTypeBits
				, VK_MEMORY_HEAP_DEVICE_LOCAL_BIT );
			VkDeviceMemory deviceMemory;
			allocate( deviceMemory
				, nullptr
				, device
				, VkMemoryAllocateInfo{ VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO, nullptr, requirements.size, deduced } );
			get( deviceMemory )->bindToImage( result, 0u );
			return result;
		}

		VkImageView createImageView( VkDevice device
			, VkImage image
			, VkFormat format )
		{
			VkImageView result;
			allocate( result
				, nullptr
				, device
				, VkImageViewCreateInfo
				{
					VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
					nullptr,
					0,
					image,
					VK_IMAGE_VIEW_TYPE_2D,
					format,
					VkComponentMapping{},
					VkImageSubresourceRange{ VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT, 0u, 1u, 0u, 1u },
				} );
			return result;
		}
	}

	SwapchainKHR::SwapchainKHR( VkDevice device
		, VkSwapchainCreateInfoKHR createInfo )
		: m_device{ device }
		, m_flags{ createInfo.flags }
		, m_surface{ createInfo.surface }
		, m_minImageCount{ createInfo.minImageCount }
		, m_imageFormat{ createInfo.imageFormat }
		, m_imageColorSpace{ createInfo.imageColorSpace }
		, m_imageExtent{ createInfo.imageExtent }
		, m_imageArrayLayers{ createInfo.imageArrayLayers }
		, m_imageUsage{ createInfo.imageUsage }
		, m_imageSharingMode{ createInfo.imageSharingMode }
		, m_queueFamilyIndices{ makeVector( createInfo.pQueueFamilyIndices, createInfo.queueFamilyIndexCount ) }
		, m_preTransform{ createInfo.preTransform }
		, m_compositeAlpha{ createInfo.compositeAlpha }
		, m_presentMode{ createInfo.presentMode }
		, m_clipped{ createInfo.clipped }
	{
		get( m_device )->registerContext( m_surface );
		m_imageExtent.height = std::max( 1u, m_imageExtent.height );
		m_imageExtent.width = std::max( 1u, m_imageExtent.width );

		m_image = createImage( device, m_imageFormat, m_imageExtent );
		m_view = createImageView( device, m_image, m_imageFormat );
		auto context = get( m_device )->getContext();
		glLogCall( context
			, glGenFramebuffers
			, 1
			, &m_fbo );
		glLogCall( context
			, glBindFramebuffer
			, GL_FRAMEBUFFER
			, m_fbo );
		glLogCall( context
			, glFramebufferTexture2D
			, GL_FRAMEBUFFER
			, GL_ATTACHMENT_POINT_COLOR0
			, GL_TEXTURE_2D
			, get( m_view )->getInternal()
			, 0u );
		checkCompleteness( context->glCheckFramebufferStatus( GL_FRAMEBUFFER ) );
		glLogCall( context
			, glBindFramebuffer
			, GL_FRAMEBUFFER
			, 0 );
	}

	SwapchainKHR::~SwapchainKHR()
	{
		auto context = get( m_device )->getContext();
		glLogCall( context
			, glDeleteFramebuffers
			, 1
			, &m_fbo );
		get( m_device )->unregisterContext( m_surface );
	}

	uint32_t SwapchainKHR::getImageCount()const
	{
		return 1u;
	}

	VkImageArray SwapchainKHR::getImages()const
	{
		VkImageArray result;
		VkImage image;
		allocate( image
			, nullptr
			, m_device
			, m_image );
		result.emplace_back( image );
		return result;
	}

	VkResult SwapchainKHR::acquireNextImage( uint64_t timeout
		, VkSemaphore semaphore
		, VkFence fence
		, uint32_t & imageIndex )const
	{
		imageIndex = 0u;
		return VK_SUCCESS;
	}

	VkResult SwapchainKHR::present( uint32_t imageIndex )const
	{
		auto context = get( m_device )->getContext();
		glLogCall( context
			, glBindFramebuffer
			, GL_READ_FRAMEBUFFER
			, m_fbo );
		glLogCall( context
			, glBindFramebuffer
			, GL_DRAW_FRAMEBUFFER
			, 0 );
		glLogCall( context
			, glBlitFramebuffer
			, 0, 0, m_imageExtent.width, m_imageExtent.height
			, 0, 0, m_imageExtent.width, m_imageExtent.height
			, GL_COLOR_BUFFER_BIT, GL_FILTER_NEAREST );
		glLogCall( context
			, glBindFramebuffer
			, GL_READ_FRAMEBUFFER
			, 0 );
		context->swapBuffers();
		return VK_SUCCESS;
	}
}
