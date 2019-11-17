/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Core/GlSurface.hpp"

#include "Core/GlInstance.hpp"
#include "Core/GlPhysicalDevice.hpp"

#include "ashesgl3_api.hpp"

namespace ashes::gl3
{
#if _WIN32

	SurfaceKHR::SurfaceKHR( VkInstance instance
		, VkWin32SurfaceCreateInfoKHR createInfo )
		: m_win32CreateInfo{ createInfo }
		, m_context{ get( instance )->createContext( std::move( createInfo ) ) }
	{
		m_presentModes.push_back( VK_PRESENT_MODE_FIFO_KHR );
		getSurfaceInfos( m_surfaceFormats, m_surfaceCapabilities );
	}

#elif __linux__

	SurfaceKHR::SurfaceKHR( VkInstance instance
		, VkXlibSurfaceCreateInfoKHR createInfo )
		: m_xlibCreateInfo{ createInfo }
		, m_context{ get( instance )->createContext( std::move( createInfo ) ) }
	{
		m_presentModes.push_back( VK_PRESENT_MODE_FIFO_KHR );
		getSurfaceInfos( m_surfaceFormats, m_surfaceCapabilities );
	}

	SurfaceKHR::SurfaceKHR( VkInstance instance
		, VkXcbSurfaceCreateInfoKHR createInfo )
		: m_xcbCreateInfo{ createInfo }
		, m_context{ get( instance )->createContext( std::move( createInfo ) ) }
	{
		m_presentModes.push_back( VK_PRESENT_MODE_FIFO_KHR );
		getSurfaceInfos( m_surfaceFormats, m_surfaceCapabilities );
	}

#endif

	void SurfaceKHR::getSurfaceInfos( VkSurfaceFormatArrayKHR & formats
		, VkSurfaceCapabilitiesKHR & capabilities )
	{
		formats.push_back( { VK_FORMAT_R8G8B8A8_UNORM, VK_COLORSPACE_SRGB_NONLINEAR_KHR } );

		capabilities.minImageCount = 1u;
		capabilities.maxImageCount = 1u;
		capabilities.currentExtent.width = ~( 0u );
		capabilities.currentExtent.height = ~( 0u );
		capabilities.minImageExtent = capabilities.currentExtent;
		capabilities.maxImageExtent = capabilities.currentExtent;
		capabilities.maxImageArrayLayers = 1u;
		capabilities.supportedUsageFlags = 0u;
		capabilities.supportedTransforms = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
		capabilities.currentTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
		capabilities.supportedCompositeAlpha = VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR;
	}
}
