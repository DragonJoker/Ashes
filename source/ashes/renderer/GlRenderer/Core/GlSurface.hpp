/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/GlRenderer/Core/GlContextLock.hpp"

namespace ashes::gl
{
	class SurfaceKHR
		: public AutoIdIcdObject< SurfaceKHR >
	{
	public:
#if _WIN32
		SurfaceKHR( VkInstance instance
			, VkWin32SurfaceCreateInfoKHR createInfo );
#elif __linux__
		SurfaceKHR( VkInstance instance
			, VkXlibSurfaceCreateInfoKHR createInfo );
		SurfaceKHR( VkInstance instance
			, VkXcbSurfaceCreateInfoKHR createInfo );
		SurfaceKHR( VkInstance instance
			, VkWaylandSurfaceCreateInfoKHR createInfo );
#elif __APPLE__
		SurfaceKHR( VkInstance instance
			, VkMacOSSurfaceCreateInfoMVK createInfo );
#endif
#ifdef VK_KHR_display
		SurfaceKHR( VkInstance instance
			, VkDisplaySurfaceCreateInfoKHR createInfo );
#endif
		~SurfaceKHR();

		VkSurfaceCapabilitiesKHR getCapabilities()const
		{
			return m_surfaceCapabilities;
		}

		VkPresentModeArrayKHR getPresentModes()const
		{
			return m_presentModes;
		}

		VkSurfaceFormatArrayKHR getFormats()const
		{
			return m_surfaceFormats;
		}

		inline ContextLock getContext()const
		{
			return { *m_context };
		}

#if _WIN32

		inline VkWin32SurfaceCreateInfoKHR getWin32CreateInfo()const
		{
			return m_win32CreateInfo;
		}

		inline bool isWin32()const
		{
			return m_win32CreateInfo.sType != 0;
		}

#elif __linux__

		inline VkXlibSurfaceCreateInfoKHR getXlibCreateInfo()const
		{
			return m_xlibCreateInfo;
		}

		inline VkXcbSurfaceCreateInfoKHR getXcbCreateInfo()const
		{
			return m_xcbCreateInfo;
		}

		inline VkWaylandSurfaceCreateInfoKHR getWaylandCreateInfo()const
		{
			return m_waylandCreateInfo;
		}

		inline bool isXcb()const
		{
			return m_xcbCreateInfo.sType != 0;
		}

		inline bool isXlib()const
		{
			return m_xlibCreateInfo.sType != 0;
		}

		inline bool isWayland()const
		{
			return m_waylandCreateInfo.sType != 0;
		}

#elif __APPLE__

		inline VkMacOSSurfaceCreateInfoMVK getMacOSCreateInfo()const
		{
			return m_macOSCreateInfo;
		}

		inline bool isMacOS()const
		{
			return m_macOSCreateInfo.sType != 0;
		}

#endif
#ifdef VK_KHR_display

		inline VkDisplaySurfaceCreateInfoKHR getDisplayCreateInfo()const
		{
			return m_displayCreateInfo;
		}

		inline bool isDisplay()const
		{
			return m_displayCreateInfo.sType != 0;
		}

#endif

		inline VkInstance getInstance()const
		{
			return m_context->getInstance();
		}

	private:
		static void getSurfaceInfos( VkSurfaceFormatArrayKHR & formats
			, VkSurfaceCapabilitiesKHR & capabilities );

	private:
		VkInstance m_instance;
#if _WIN32
		VkWin32SurfaceCreateInfoKHR m_win32CreateInfo{};
#elif __linux__
		VkXlibSurfaceCreateInfoKHR m_xlibCreateInfo{};
		VkXcbSurfaceCreateInfoKHR m_xcbCreateInfo{};
		VkWaylandSurfaceCreateInfoKHR m_waylandCreateInfo{};
#elif __APPLE__
		VkMacOSSurfaceCreateInfoMVK m_macOSCreateInfo{};
#endif
#ifdef VK_KHR_display
		VkDisplaySurfaceCreateInfoKHR m_displayCreateInfo{};
#endif
		VkSurfaceFormatArrayKHR m_surfaceFormats;
		VkSurfaceCapabilitiesKHR m_surfaceCapabilities;
		VkPresentModeArrayKHR m_presentModes;
		ContextPtr m_context;
	};
}
