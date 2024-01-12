#include "Core/GlSwapChain.hpp"

#include "Core/GlDevice.hpp"
#include "Image/GlImage.hpp"
#include "Image/GlImageView.hpp"
#include "Miscellaneous/GlCallLogger.hpp"
#include "RenderPass/GlFrameBuffer.hpp"

#include "ashesgl_api.hpp"

namespace ashes::gl
{
	namespace
	{
		VkImage createImage( VkDevice device
			, VkAllocationCallbacks const * allocInfo
			, VkFormat format
			, VkExtent2D dimensions
			, VkDeviceMemory & deviceMemory )
		{
			VkImage result;
			auto err = allocate( result
				, allocInfo
				, device
				, VkImageCreateInfo{ VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO
					, nullptr
					, 0u
					, VK_IMAGE_TYPE_2D
					, format
					, VkExtent3D{ dimensions.width, dimensions.height, 1u }
					, 1u
					, 1u
					, VK_SAMPLE_COUNT_1_BIT
					, VK_IMAGE_TILING_OPTIMAL
					, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT
					, {} // sharingMode
					, {} // queueFamilyIndexCount
					, {} // pQueueFamilyIndices
					, {} // initialLayout
				}
				, true );

			if ( err != VK_SUCCESS )
			{
				throw ashes::Exception{ err, "Swapchain image allocation" };
			}

#if VK_EXT_debug_utils
			get( device )->setDebugUtilsObjectName( { VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT
				, nullptr
				, VK_OBJECT_TYPE_IMAGE
				, uint64_t( result )
				, "SwapchainImage" } );
#elif VK_EXT_debug_marker
			get( device )->debugMarkerSetObjectName( { VK_STRUCTURE_TYPE_DEBUG_MARKER_OBJECT_NAME_INFO_EXT
				, nullptr
				, VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_EXT
				, uint64_t( result )
				, "SwapchainImage" } )const;
#endif

			auto requirements = get( result )->getMemoryRequirements();
			uint32_t deduced = deduceMemoryType( device
				, requirements.memoryTypeBits
				, VK_MEMORY_HEAP_DEVICE_LOCAL_BIT );
			err = allocate( deviceMemory
				, allocInfo
				, device
				, VkMemoryAllocateInfo{ VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO, nullptr, requirements.size, deduced } );

			if ( err != VK_SUCCESS )
			{
				deallocate( result
					, allocInfo );
				throw ashes::Exception{ err, "Swapchain image memory allocation" };
			}

#if VK_EXT_debug_utils
			get( device )->setDebugUtilsObjectName( { VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT
				, nullptr
				, VK_OBJECT_TYPE_DEVICE_MEMORY
				, uint64_t( result )
				, "SwapchainImageMem" } );
#elif VK_EXT_debug_marker
			get( device )->debugMarkerSetObjectName( { VK_STRUCTURE_TYPE_DEBUG_MARKER_OBJECT_NAME_INFO_EXT
				, nullptr
				, VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_MEMORY_EXT
				, uint64_t( result )
				, "SwapchainImageMem" } )const;
#endif

			get( deviceMemory )->bindImage( result, 0u );
			return result;
		}

		VkImageView createImageView( VkDevice device
			, VkAllocationCallbacks const * allocInfo
			, VkImage image
			, VkFormat format )
		{
			VkImageView result{};

			if ( auto err = allocate( result
					, allocInfo
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
				err != VK_SUCCESS )
			{
				throw ashes::Exception{ err, "Swapchain image allocation" };
			}

#if VK_EXT_debug_utils
			get( device )->setDebugUtilsObjectName( { VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT
				, nullptr
				, VK_OBJECT_TYPE_IMAGE_VIEW
				, uint64_t( result )
				, "SwapchainImageView" } );
#elif VK_EXT_debug_marker
			get( device )->debugMarkerSetObjectName( { VK_STRUCTURE_TYPE_DEBUG_MARKER_OBJECT_NAME_INFO_EXT
				, nullptr
				, VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_VIEW_EXT
				, uint64_t( result )
				, "SwapchainImageView" } )const;
#endif

			return result;
		}
	}

	SwapchainKHR::SwapchainKHR( VkAllocationCallbacks const * allocInfo
		, VkDevice device
		, VkSwapchainCreateInfoKHR createInfo )
		: m_allocInfo{ allocInfo }
		, m_device{ device }
		, m_createInfo{ std::move( createInfo ) }
	{
		get( m_device )->link( m_createInfo.surface );
		m_createInfo.imageExtent.height = std::max( 1u, m_createInfo.imageExtent.height );
		m_createInfo.imageExtent.width = std::max( 1u, m_createInfo.imageExtent.width );

		m_image = createImage( device
			, m_allocInfo
			, m_createInfo.imageFormat
			, m_createInfo.imageExtent
			, m_deviceMemory );

		if ( hasTextureViews( device ) )
		{
			try
			{
				m_view = createImageView( device
					, m_allocInfo
					, m_image
					, m_createInfo.imageFormat );
			}
			catch ( ashes::Exception & )
			{
				deallocate( m_deviceMemory
					, m_allocInfo );
				deallocate( m_image
					, m_allocInfo );
				throw;
			}
		}

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
			, ( hasTextureViews( device )
				? get( m_view )->getInternal()
				: get( m_image )->getInternal() )
			, 0u );
		checkCompleteness( get( this )
			, context->glCheckFramebufferStatus( GL_FRAMEBUFFER ) );
		glLogCall( context
			, glBindFramebuffer
			, GL_FRAMEBUFFER
			, 0 );
		registerObject( m_device, *this );
	}

	SwapchainKHR::~SwapchainKHR()noexcept
	{
		unregisterObject( m_device, *this );
		{
			auto context = get( m_device )->getContext();
			glLogCall( context
				, glDeleteFramebuffers
				, 1
				, &m_internal );

			if ( hasTextureViews( m_device ) )
			{
				deallocate( m_view
					, m_allocInfo );
			}

			deallocate( m_deviceMemory
				, m_allocInfo );
			deallocate( m_image
				, m_allocInfo );
		}
		get( m_device )->unlink();
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

	VkResult SwapchainKHR::acquireNextImage( uint32_t & imageIndex )const
	{
		imageIndex = 0u;
		return VK_SUCCESS;
	}

	VkResult SwapchainKHR::present()const
	{
		auto srcExtent = m_createInfo.imageExtent;
		auto dstExtent = m_createInfo.imageExtent;

		if ( get( m_createInfo.surface )->isDisplay() )
		{
			dstExtent = get( m_createInfo.surface )->getDisplayCreateInfo().imageExtent;
		}

		auto context = get( m_device )->getContext();

		if ( context->hasPushDebugGroup() )
		{
			glLogCall( context
				, glPushDebugGroup
				, GL_DEBUG_SOURCE_APPLICATION
				, 1u
				, -1
				, "Swapchain final swap" );
		}

		glLogCall( context
			, glBindFramebuffer
			, GL_READ_FRAMEBUFFER
			, m_internal );
		glLogCall( context
			, glFramebufferTexture2D
			, GL_READ_FRAMEBUFFER
			, GL_ATTACHMENT_POINT_COLOR0
			, GL_TEXTURE_2D
			, ( hasTextureViews( m_device )
				? get( m_view )->getInternal()
				: get( m_image )->getInternal() )
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
			, 0, 0, GLint( srcExtent.width ), GLint( srcExtent.height )
			, 0, 0, GLint( dstExtent.width ), GLint( dstExtent.height )
			, GL_COLOR_BUFFER_BIT, GL_FILTER_LINEAR );
		glLogCall( context
			, glBindFramebuffer
			, GL_READ_FRAMEBUFFER
			, 0 );
		context->swapBuffers();

		if ( context->hasPushDebugGroup() )
		{
			glLogEmptyCall( context
				, glPopDebugGroup );
		}

		return VK_SUCCESS;
	}
}
