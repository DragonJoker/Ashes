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


		inline std::vector< DXGI_MODE_DESC > const & getDescs( VkFormat format )const
		{
			return m_descs[format];
		}

	private:
		void getSurfaceInfos();

	private:
		VkSurfaceCreateInfoKHR m_createInfo;
		VkSurfaceFormatArrayKHR m_surfaceFormats;
		VkSurfaceCapabilitiesKHR m_surfaceCapabilities;
		VkPresentModeArrayKHR m_presentModes;
		std::string m_type;
		std::vector< DXGI_MODE_DESC > m_displayModes;
		mutable std::map< VkFormat, std::vector< DXGI_MODE_DESC > > m_descs;
	};
}
