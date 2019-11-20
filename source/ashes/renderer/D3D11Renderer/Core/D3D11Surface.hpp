/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/D3D11Renderer/D3D11RendererPrerequisites.hpp"

namespace ashes::d3d11
{
	std::vector< DXGI_MODE_DESC > getDisplayModesList( VkInstance instance
		, IDXGIOutput * adapterOutput );

	class SurfaceKHR
	{
	public:
		SurfaceKHR( VkInstance instance
			, VkWin32SurfaceCreateInfoKHR createInfo );
		SurfaceKHR( VkInstance instance
			, VkDisplaySurfaceCreateInfoKHR createInfo );
		~SurfaceKHR();

		bool getSupport( uint32_t queueFamilyIndex )const;
		HWND getHwnd()const;
		DXGI_MODE_DESC const & getDisplayMode()const;

		VkSurfaceCapabilitiesKHR getCapabilities( VkPhysicalDevice physicalDevice )const
		{
			m_currentPhysicalDevice = VK_NULL_HANDLE;
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

		VkInstance getInstance()const
		{
			return m_instance;
		}

		inline std::vector< DXGI_MODE_DESC > const & getDescs( VkFormat format )const
		{
			return m_descs[format];
		}

		inline bool isWin32()const
		{
			return m_win32CreateInfo.sType != 0;
		}

		inline bool isDisplay()const
		{
			return m_displayCreateInfo.sType != 0;
		}

	private:
		void doUpdate( VkPhysicalDevice physicalDevice )const;

	private:
		VkInstance m_instance;
		VkWin32SurfaceCreateInfoKHR m_win32CreateInfo{};
		VkDisplaySurfaceCreateInfoKHR m_displayCreateInfo{};
		std::string m_type;
		mutable VkSurfaceFormatArrayKHR m_surfaceFormats;
		mutable VkSurfaceCapabilitiesKHR m_surfaceCapabilities;
		mutable VkPresentModeArrayKHR m_presentModes;
		mutable VkPhysicalDevice m_currentPhysicalDevice{ VK_NULL_HANDLE };
		mutable std::vector< DXGI_MODE_DESC > m_displayModes;
		mutable std::map< VkFormat, std::vector< DXGI_MODE_DESC > > m_descs;
	};
}
