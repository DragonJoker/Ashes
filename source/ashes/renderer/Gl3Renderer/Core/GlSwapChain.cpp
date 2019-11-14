#include "Core/GlSwapChain.hpp"

#include "Core/GlDevice.hpp"
#include "Image/GlImage.hpp"
#include "Image/GlImageView.hpp"
#include "Miscellaneous/GlCallLogger.hpp"
#include "RenderPass/GlFrameBuffer.hpp"

#include "ashesgl3_api.hpp"

namespace ashes::gl3
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
	}

	SwapchainKHR::SwapchainKHR( VkDevice device
		, VkSwapchainCreateInfoKHR createInfo )
		: m_device{ device }
		, m_createInfo{ createInfo }
	{
		get( m_device )->registerContext( m_createInfo.surface );
		m_createInfo.imageExtent.height = std::max( 1u, m_createInfo.imageExtent.height );
		m_createInfo.imageExtent.width = std::max( 1u, m_createInfo.imageExtent.width );

		m_image = createImage( device
			, m_createInfo.imageFormat
			, m_createInfo.imageExtent
			, m_deviceMemory );
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
			, get( m_image )->getInternal()
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
		deallocate( m_deviceMemory, nullptr );
		deallocate( m_image, nullptr );
		get( m_device )->unregisterContext( m_createInfo.surface );
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
		auto context = get( m_device )->getContext();
		glLogCall( context
			, glBindFramebuffer
			, GL_READ_FRAMEBUFFER
			, m_fbo );
		glLogCall( context
			, glFramebufferTexture2D
			, GL_READ_FRAMEBUFFER
			, GL_ATTACHMENT_POINT_COLOR0
			, GL_TEXTURE_2D
			, get( m_image )->getInternal()
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
			, 0, 0, m_createInfo.imageExtent.width, m_createInfo.imageExtent.height
			, 0, 0, m_createInfo.imageExtent.width, m_createInfo.imageExtent.height
			, GL_COLOR_BUFFER_BIT, GL_FILTER_NEAREST );
		glLogCall( context
			, glBindFramebuffer
			, GL_READ_FRAMEBUFFER
			, 0 );
		context->swapBuffers();
		return VK_SUCCESS;
	}
}
