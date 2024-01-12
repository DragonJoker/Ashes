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
		SurfaceKHR( VkAllocationCallbacks const * allocInfo
			, VkInstance instance
			, VkWin32SurfaceCreateInfoKHR createInfo );
#elif __linux__
		SurfaceKHR( VkAllocationCallbacks const * allocInfo
			, VkInstance instance
			, VkXlibSurfaceCreateInfoKHR createInfo );
		SurfaceKHR( VkAllocationCallbacks const * allocInfo
			, VkInstance instance
			, VkXcbSurfaceCreateInfoKHR createInfo );
		SurfaceKHR( VkAllocationCallbacks const * allocInfo
			, VkInstance instance
			, VkWaylandSurfaceCreateInfoKHR createInfo );
#elif __APPLE__
		SurfaceKHR( VkAllocationCallbacks const * allocInfo
			, VkInstance instance
			, VkMacOSSurfaceCreateInfoMVK createInfo );
#endif
#ifdef VK_KHR_display
		SurfaceKHR( VkAllocationCallbacks const * allocInfo
			, VkInstance instance
			, VkDisplaySurfaceCreateInfoKHR createInfo );
#endif
		~SurfaceKHR()noexcept;

		VkSurfaceCapabilitiesKHR getCapabilities()
		{
			updateSurfaceInfos();
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

		ContextLock getContext()const
		{
			return { *m_context };
		}

#if _WIN32

		VkWin32SurfaceCreateInfoKHR getWin32CreateInfo()const
		{
			return m_win32CreateInfo;
		}

		bool isWin32()const
		{
			return m_win32CreateInfo.sType != 0;
		}

#elif __linux__

		VkXlibSurfaceCreateInfoKHR getXlibCreateInfo()const
		{
			return m_xlibCreateInfo;
		}

		VkXcbSurfaceCreateInfoKHR getXcbCreateInfo()const
		{
			return m_xcbCreateInfo;
		}

		VkWaylandSurfaceCreateInfoKHR getWaylandCreateInfo()const
		{
			return m_waylandCreateInfo;
		}

		bool isXcb()const
		{
			return m_xcbCreateInfo.sType != 0;
		}

		bool isXlib()const
		{
			return m_xlibCreateInfo.sType != 0;
		}

		bool isWayland()const
		{
			return m_waylandCreateInfo.sType != 0;
		}

#elif __APPLE__

		VkMacOSSurfaceCreateInfoMVK getMacOSCreateInfo()const
		{
			return m_macOSCreateInfo;
		}

		bool isMacOS()const
		{
			return m_macOSCreateInfo.sType != 0;
		}

#endif
#ifdef VK_KHR_display

		VkDisplaySurfaceCreateInfoKHR getDisplayCreateInfo()const
		{
			return m_displayCreateInfo;
		}

		bool isDisplay()const
		{
			return m_displayCreateInfo.sType != 0;
		}

#endif

		VkInstance getInstance()const
		{
			return m_context->getInstance();
		}

	private:
		void updateSurfaceInfos();
		static void getDefaultSurfaceInfos( VkSurfaceFormatArrayKHR & formats
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
