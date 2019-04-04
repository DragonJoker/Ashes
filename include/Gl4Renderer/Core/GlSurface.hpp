/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "Gl4Renderer/Core/GlContext.hpp"

#include <AshesCommon/WindowHandle.hpp>

namespace ashes::gl4
{
	class SurfaceKHR
	{
	public:
		template< typename CreateInfoT >
		inline SurfaceKHR( VkInstance instance
			, CreateInfoT createInfo )
			: m_context{ Context::create( instance
				, createInfo
				, &get( instance )->getContext() ) }
		{
			m_presentModes.push_back( VK_PRESENT_MODE_FIFO_KHR );
			getSurfaceInfos( m_surfaceFormats, m_surfaceCapabilities );
		}

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
		std::string m_type;
	};
}
