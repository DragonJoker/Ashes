#include "Core/GlSwapChain.hpp"

#include "Core/GlDevice.hpp"
#include "Image/GlImage.hpp"
#include "Image/GlImageView.hpp"
#include "Miscellaneous/GlCallLogger.hpp"
#include "RenderPass/GlFrameBuffer.hpp"

#include "ashesgl4_api.hpp"

namespace ashes::gl4
{
	namespace
	{
		VkImage createImage( VkDevice device
			, VkFormat format
			, VkExtent2D dimensions
			, VkDeviceMemory & deviceMemory )
		{
			VkImage result;
			allocate( result
				, nullptr
				, device
				, format
				, std::move( dimensions )
				, true );
			auto requirements = get( result )->getMemoryRequirements();
			uint32_t deduced = deduceMemoryType( requirements.memoryTypeBits
				, VK_MEMORY_HEAP_DEVICE_LOCAL_BIT );
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
		, m_createInfo{ createInfo }
	{
		get( m_device )->link( m_createInfo.surface );
		m_createInfo.imageExtent.height = std::max( 1u, m_createInfo.imageExtent.height );
		m_createInfo.imageExtent.width = std::max( 1u, m_createInfo.imageExtent.width );

		m_image = createImage( device
			, m_createInfo.imageFormat
			, m_createInfo.imageExtent
			, m_deviceMemory );
		m_view = createImageView( device
			, m_image
			, m_createInfo.imageFormat );
		auto context = get( m_device )->getContext();
		glLogCall( context
			, glGenFramebuffers
			, 1
			, &m_internal );
		glLogCall( context
			, glBindFramebuffer
			, GL_FRAMEBUFFER
			, m_internal );
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
		{
			auto context = get( m_device )->getContext();
			glLogCall( context
				, glDeleteFramebuffers
				, 1
				, &m_internal );
			deallocate( m_view, nullptr );
			deallocate( m_deviceMemory, nullptr );
			deallocate( m_image, nullptr );
		}
		get( m_device )->unlink( m_createInfo.surface );
	}

	uint32_t SwapchainKHR::getImageCount()const
	{
		return 1u;
	}

	VkImageArray SwapchainKHR::getImages()const
	{
		VkImageArray result;
		result.emplace_back( m_image );
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
		auto srcExtent = m_createInfo.imageExtent;
		auto dstExtent = m_createInfo.imageExtent;

		if ( get( m_createInfo.surface )->isDisplay() )
		{
			dstExtent = get( m_createInfo.surface )->getDisplayCreateInfo().imageExtent;
		}

		auto context = get( m_device )->getContext();
		glLogCall( context
			, glBindFramebuffer
			, GL_READ_FRAMEBUFFER
			, m_internal );
		glLogCall( context
			, glFramebufferTexture2D
			, GL_READ_FRAMEBUFFER
			, GL_ATTACHMENT_POINT_COLOR0
			, GL_TEXTURE_2D
			, get( m_view )->getInternal()
			, 0u );
		glLogCall( context
			, glReadBuffer
			, GL_ATTACHMENT_POINT_COLOR0 );
		glLogCall( context
			, glBindFramebuffer
			, GL_DRAW_FRAMEBUFFER
			, 0 );
		glLogCall( context
			, glBlitFramebuffer
			, 0, 0, srcExtent.width, srcExtent.height
			, 0, 0, dstExtent.width, dstExtent.height
			, GL_COLOR_BUFFER_BIT, GL_FILTER_LINEAR );
		glLogCall( context
			, glBindFramebuffer
			, GL_READ_FRAMEBUFFER
			, 0 );
		context->swapBuffers();
		return VK_SUCCESS;
	}
}
