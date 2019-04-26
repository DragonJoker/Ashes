/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/D3D11Renderer/D3D11RendererPrerequisites.hpp"

namespace ashes::d3d11
{
	class SurfaceKHR
	{
	public:
		SurfaceKHR( VkInstance instance
			, VkSurfaceCreateInfoKHR createInfo );
		~SurfaceKHR();

		bool getSupport( uint32_t queueFamilyIndex )const;

		VkSurfaceCapabilitiesKHR getCapabilities( VkPhysicalDevice physicalDevice )const
		{
			doUpdate( physicalDevice );
			return m_surfaceCapabilities;
		}

		VkPresentModeArrayKHR getPresentModes( VkPhysicalDevice physicalDevice )const
		{
			doUpdate( physicalDevice );
			return m_presentModes;
		}

		VkSurfaceFormatArrayKHR getFormats( VkPhysicalDevice physicalDevice )const
		{
			doUpdate( physicalDevice );
			return m_surfaceFormats;
		}

		HWND getHwnd()const
		{
			return m_createInfo.hwnd;
		}

		inline std::vector< DXGI_MODE_DESC > const & getDescs( VkFormat format )const
		{
			return m_descs[format];
		}

	private:
		void doUpdate( VkPhysicalDevice physicalDevice )const;

	private:
		VkSurfaceCreateInfoKHR m_createInfo;
		std::string m_type;
		mutable VkSurfaceFormatArrayKHR m_surfaceFormats;
		mutable VkSurfaceCapabilitiesKHR m_surfaceCapabilities;
		mutable VkPresentModeArrayKHR m_presentModes;
		mutable VkPhysicalDevice m_currentPhysicalDevice{ VK_NULL_HANDLE };
		mutable std::vector< DXGI_MODE_DESC > m_displayModes;
		mutable std::map< VkFormat, std::vector< DXGI_MODE_DESC > > m_descs;
	};
}
