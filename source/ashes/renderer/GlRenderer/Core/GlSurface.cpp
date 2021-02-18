/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Core/GlSurface.hpp"

#include "Core/GlInstance.hpp"
#include "Core/GlPhysicalDevice.hpp"

#include "ashesgl_api.hpp"

namespace ashes::gl
{
#if _WIN32

	SurfaceKHR::SurfaceKHR( VkInstance instance
		, VkWin32SurfaceCreateInfoKHR createInfo )
		: m_instance{ instance }
		, m_win32CreateInfo{ createInfo }
	{
		m_context = get( m_instance )->registerSurface( get( this ) );
		m_presentModes.push_back( VK_PRESENT_MODE_FIFO_KHR );
		getSurfaceInfos( m_surfaceFormats, m_surfaceCapabilities );
	}

#elif __linux__

	SurfaceKHR::SurfaceKHR( VkInstance instance
		, VkXlibSurfaceCreateInfoKHR createInfo )
		: m_instance{ instance }
		, m_xlibCreateInfo{ createInfo }
	{
		m_context = get( m_instance )->registerSurface( get( this ) );
		m_presentModes.push_back( VK_PRESENT_MODE_FIFO_KHR );
		getSurfaceInfos( m_surfaceFormats, m_surfaceCapabilities );
	}

	SurfaceKHR::SurfaceKHR( VkInstance instance
		, VkXcbSurfaceCreateInfoKHR createInfo )
		: m_instance{ instance }
		, m_xcbCreateInfo{ createInfo }
	{
		m_context = get( m_instance )->registerSurface( get( this ) );
		m_presentModes.push_back( VK_PRESENT_MODE_FIFO_KHR );
		getSurfaceInfos( m_surfaceFormats, m_surfaceCapabilities );
	}

	SurfaceKHR::SurfaceKHR( VkInstance instance
		, VkWaylandSurfaceCreateInfoKHR createInfo )
		: m_instance{ instance }
		, m_waylandCreateInfo{ createInfo }
	{
		m_context = get( m_instance )->registerSurface( get( this ) );
		m_presentModes.push_back( VK_PRESENT_MODE_FIFO_KHR );
		getSurfaceInfos( m_surfaceFormats, m_surfaceCapabilities );
	}

#elif __APPLE__

	SurfaceKHR::SurfaceKHR( VkInstance instance
		, VkMacOSSurfaceCreateInfoMVK createInfo )
		: m_instance{ instance }
		, m_macOSCreateInfo{ createInfo }
	{
		m_context = get( m_instance )->registerSurface( get( this ) );
		m_presentModes.push_back( VK_PRESENT_MODE_FIFO_KHR );
		getSurfaceInfos( m_surfaceFormats, m_surfaceCapabilities );
	}

#endif
#ifdef VK_KHR_display

	SurfaceKHR::SurfaceKHR( VkInstance instance
		, VkDisplaySurfaceCreateInfoKHR createInfo )
		: m_instance{ instance }
		, m_displayCreateInfo{ std::move( createInfo ) }
	{
		m_context = get( m_instance )->registerSurface( get( this ) );
		m_displayCreateInfo.imageExtent = m_context->getExtent();
		m_presentModes.push_back( VK_PRESENT_MODE_FIFO_KHR );
		getSurfaceInfos( m_surfaceFormats, m_surfaceCapabilities );
	}

#endif

	SurfaceKHR::~SurfaceKHR()
	{
		get( m_instance )->unregisterSurface( get( this ) );
	}

	void SurfaceKHR::getSurfaceInfos( VkSurfaceFormatArrayKHR & formats
		, VkSurfaceCapabilitiesKHR & capabilities )
	{
		formats.push_back( { VK_FORMAT_R8G8B8A8_UNORM, VK_COLORSPACE_SRGB_NONLINEAR_KHR } );

		capabilities.minImageCount = 1u;
		capabilities.maxImageCount = 1u;
		capabilities.currentExtent.width = (uint32_t)-1;
		capabilities.currentExtent.height = (uint32_t)-1;
		capabilities.minImageExtent = { 1u, 1u };
		capabilities.maxImageExtent = { 65536u, 65536u };
		capabilities.maxImageArrayLayers = 1u;
		capabilities.supportedUsageFlags = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		capabilities.supportedTransforms = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
		capabilities.currentTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
		capabilities.supportedCompositeAlpha = VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR;
	}
}
