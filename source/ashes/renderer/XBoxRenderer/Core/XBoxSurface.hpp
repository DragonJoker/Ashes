/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/XBoxRenderer/XBoxRendererPrerequisites.hpp"

namespace ashes::xbox
{
	std::vector< DXGI_MODE_DESC > getDisplayModesList( VkInstance instance
		, IDXGIOutput * adapterOutput );

	class SurfaceKHR
	{
	public:
		SurfaceKHR( VkInstance instance
			, VkWin32SurfaceCreateInfoKHR createInfo );

		VkBool32 getSupport( VkPhysicalDevice physicalDevice )const;
		HWND getHwnd()const;

		VkSurfaceCapabilitiesKHR getCapabilities( VkPhysicalDevice physicalDevice )const
		{
			m_currentPhysicalDevice = nullptr;
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

		std::vector< DXGI_MODE_DESC > const & getDescs( VkFormat format )const
		{
			return m_descs[format];
		}

		DXGI_MODE_DESC const & getMatchingDesc( VkFormat format )const
		{
			return m_matchingDescs[format];
		}

	private:
		void doUpdate( VkPhysicalDevice physicalDevice )const;

	private:
		VkInstance m_instance{};
		VkWin32SurfaceCreateInfoKHR m_createInfo{};
		std::string m_type;
		mutable VkSurfaceFormatArrayKHR m_surfaceFormats{};
		mutable VkSurfaceCapabilitiesKHR m_surfaceCapabilities{};
		mutable VkPresentModeArrayKHR m_presentModes{};
		mutable VkPhysicalDevice m_currentPhysicalDevice{};
		mutable std::vector< DXGI_MODE_DESC > m_displayModes{};
		mutable std::map< VkFormat, std::vector< DXGI_MODE_DESC > > m_descs{};
		mutable std::map< VkFormat, DXGI_MODE_DESC > m_matchingDescs{};
	};
}
