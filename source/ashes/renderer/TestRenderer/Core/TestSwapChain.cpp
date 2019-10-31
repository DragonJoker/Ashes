#include "Core/TestSwapChain.hpp"

#include "Core/TestDevice.hpp"
#include "Core/TestInstance.hpp"
#include "Core/TestPhysicalDevice.hpp"
#include "Core/TestSurface.hpp"
#include "Image/TestImage.hpp"
#include "RenderPass/TestFrameBuffer.hpp"
#include "RenderPass/TestRenderPass.hpp"
#include "Sync/TestSemaphore.hpp"

#include "ashestest_api.hpp"

#include <sstream>

namespace ashes::test
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
				, std::move( dimensions ) );
			auto requirements = get( result )->getMemoryRequirements();
			uint32_t deduced = deduceMemoryType( requirements.memoryTypeBits
				, VK_MEMORY_HEAP_DEVICE_LOCAL_BIT
				, get( get( device )->getGpu() )->getMemoryProperties() );
			allocate( deviceMemory
				, nullptr
				, device
				, VkMemoryAllocateInfo{ VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO, nullptr, requirements.size, deduced } );
			get( result )->bindMemory( deviceMemory, 0u );
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
		, VkSwapchainCreateInfoKHR createInfo ) try
		: m_device{ device }
		, m_createInfo{ std::move( createInfo ) }
	{
		m_image = createImage( device
			, m_createInfo.imageFormat
			, m_createInfo.imageExtent
			, m_deviceMemory );
		m_view = createImageView( device
			, m_image
			, m_createInfo.imageFormat );
	}
	catch ( std::exception & exc )
	{
		deallocate( m_view, nullptr );
		deallocate( m_image, nullptr );

		std::stringstream stream;
		stream << "Swapchain creation failed: " << exc.what() << std::endl;
		get( device )->onReportMessage( VK_DEBUG_REPORT_ERROR_BIT_EXT
			, VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_EXT
			, uint64_t( device )
			, 0u
			, VK_ERROR_INCOMPATIBLE_DRIVER
			, "Test"
			, stream.str().c_str() );
	}

	SwapchainKHR::~SwapchainKHR()
	{
		deallocate( m_view, nullptr );
		deallocate( m_image, nullptr );
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
		return VK_SUCCESS;
	}
}
