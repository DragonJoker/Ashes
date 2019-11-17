/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/Gl4Renderer/Core/GlContextLock.hpp"

namespace ashes::gl4
{
	class SurfaceKHR
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
#endif

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

#elif __linux__

		inline VkXlibSurfaceCreateInfoKHR getXlibCreateInfo()const
		{
			return m_xlibCreateInfo;
		}

		inline VkXcbSurfaceCreateInfoKHR getXcbCreateInfo()const
		{
			return m_xcbCreateInfo;
		}

		inline bool isXcb()const
		{
			return m_xcbCreateInfo.sType != 0;
		}

		inline bool isXlib()const
		{
			return m_xlibCreateInfo.sType != 0;
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
#if _WIN32
		VkWin32SurfaceCreateInfoKHR m_win32CreateInfo{};
#elif __linux__
		VkXlibSurfaceCreateInfoKHR m_xlibCreateInfo{};
		VkXcbSurfaceCreateInfoKHR m_xcbCreateInfo{};
#endif
		VkSurfaceFormatArrayKHR m_surfaceFormats;
		VkSurfaceCapabilitiesKHR m_surfaceCapabilities;
		VkPresentModeArrayKHR m_presentModes;
		ContextPtr m_context;
		std::string m_type;
	};
}
