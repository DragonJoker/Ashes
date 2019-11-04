/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/Gl3Renderer/Core/GlContextLock.hpp"

namespace ashes::gl3
{
	class SurfaceKHR
	{
	public:
		SurfaceKHR( VkInstance instance
			, VkSurfaceCreateInfoKHR createInfo );

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

	private:
		static void getSurfaceInfos( VkSurfaceFormatArrayKHR & formats
			, VkSurfaceCapabilitiesKHR & capabilities );

	private:
		VkSurfaceFormatArrayKHR m_surfaceFormats;
		VkSurfaceCapabilitiesKHR m_surfaceCapabilities;
		VkPresentModeArrayKHR m_presentModes;
		ContextPtr m_context;
	};
}
