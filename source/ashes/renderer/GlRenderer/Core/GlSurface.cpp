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

	SurfaceKHR::SurfaceKHR( [[maybe_unused]] VkAllocationCallbacks const * allocInfo
		, VkInstance instance
		, VkWin32SurfaceCreateInfoKHR createInfo )
		: m_instance{ instance }
		, m_win32CreateInfo{ std::move( createInfo ) }
	{
		m_context = get( m_instance )->registerSurface( get( this ) );
		m_presentModes.push_back( VK_PRESENT_MODE_FIFO_KHR );
		getDefaultSurfaceInfos( m_surfaceFormats, m_surfaceCapabilities );
		updateSurfaceInfos();
	}

#elif __linux__

	SurfaceKHR::SurfaceKHR( [[maybe_unused]] VkAllocationCallbacks const * allocInfo
		, VkInstance instance
		, VkXlibSurfaceCreateInfoKHR createInfo )
		: m_instance{ instance }
		, m_xlibCreateInfo{ std::move( createInfo ) }
	{
		m_context = get( m_instance )->registerSurface( get( this ) );
		m_presentModes.push_back( VK_PRESENT_MODE_FIFO_KHR );
		getDefaultSurfaceInfos( m_surfaceFormats, m_surfaceCapabilities );
		updateSurfaceInfos();
	}

	SurfaceKHR::SurfaceKHR( [[maybe_unused]] VkAllocationCallbacks const * allocInfo
		, VkInstance instance
		, VkXcbSurfaceCreateInfoKHR createInfo )
		: m_instance{ instance }
		, m_xcbCreateInfo{ std::move( createInfo ) }
	{
		m_context = get( m_instance )->registerSurface( get( this ) );
		m_presentModes.push_back( VK_PRESENT_MODE_FIFO_KHR );
		getDefaultSurfaceInfos( m_surfaceFormats, m_surfaceCapabilities );
		updateSurfaceInfos();
	}

	SurfaceKHR::SurfaceKHR( [[maybe_unused]] VkAllocationCallbacks const * allocInfo
		, VkInstance instance
		, VkWaylandSurfaceCreateInfoKHR createInfo )
		: m_instance{ instance }
		, m_waylandCreateInfo{ std::move( createInfo ) }
	{
		m_context = get( m_instance )->registerSurface( get( this ) );
		m_presentModes.push_back( VK_PRESENT_MODE_FIFO_KHR );
		getDefaultSurfaceInfos( m_surfaceFormats, m_surfaceCapabilities );
		updateSurfaceInfos();
	}

#elif __APPLE__

	SurfaceKHR::SurfaceKHR( [[maybe_unused]] VkAllocationCallbacks const * allocInfo
		, VkInstance instance
		, VkMacOSSurfaceCreateInfoMVK createInfo )
		: m_instance{ instance }
		, m_macOSCreateInfo{ std::move( createInfo ) }
	{
		m_context = get( m_instance )->registerSurface( get( this ) );
		m_presentModes.push_back( VK_PRESENT_MODE_FIFO_KHR );
		getDefaultSurfaceInfos( m_surfaceFormats, m_surfaceCapabilities );
		updateSurfaceInfos();
	}

#endif
#ifdef VK_KHR_display

	SurfaceKHR::SurfaceKHR( [[maybe_unused]] VkAllocationCallbacks const * allocInfo
		, VkInstance instance
		, VkDisplaySurfaceCreateInfoKHR createInfo )
		: m_instance{ instance }
		, m_displayCreateInfo{ std::move( createInfo ) }
	{
		m_context = get( m_instance )->registerSurface( get( this ) );
		m_displayCreateInfo.imageExtent = m_context->getExtent();
		m_presentModes.push_back( VK_PRESENT_MODE_FIFO_KHR );
		getDefaultSurfaceInfos( m_surfaceFormats, m_surfaceCapabilities );
		updateSurfaceInfos();
	}

#endif

	SurfaceKHR::~SurfaceKHR()noexcept
	{
		get( m_instance )->unregisterSurface( get( this ) );
	}

	void SurfaceKHR::updateSurfaceInfos()
	{
		m_surfaceCapabilities.currentExtent = m_context->getExtent();
	}

	void SurfaceKHR::getDefaultSurfaceInfos( VkSurfaceFormatArrayKHR & formats
		, VkSurfaceCapabilitiesKHR & capabilities )
	{
		formats.push_back( { VK_FORMAT_R8G8B8A8_UNORM, VK_COLORSPACE_SRGB_NONLINEAR_KHR } );

		capabilities.minImageCount = 1u;
		capabilities.maxImageCount = 1u;
		capabilities.currentExtent.width = ~0u;
		capabilities.currentExtent.height = ~0u;
		capabilities.minImageExtent = { 1u, 1u };
		capabilities.maxImageExtent = { 65536u, 65536u };
		capabilities.maxImageArrayLayers = 1u;
		capabilities.supportedUsageFlags = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		capabilities.supportedTransforms = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
		capabilities.currentTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
		capabilities.supportedCompositeAlpha = VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR;
	}
}
