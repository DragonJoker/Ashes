/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "D3D11Renderer/D3D11RendererPrerequisites.hpp"

#include <Ashes/Core/Surface.hpp>

namespace ashes::d3d11
{
	class Surface
		: public ashes::Surface
	{
	public:
		Surface( Instance const & instance
			, ashes::PhysicalDevice const & gpu
			, ashes::WindowHandle handle );
		~Surface();

		bool getSupport( uint32_t queueFamilyIndex )const;
		std::vector < VkPresentModeKHR > getPresentModes()const;
		ashes::SurfaceCapabilities getCapabilities()const;
		std::vector< VkSurfaceFormatKHR > getFormats()const;

		inline std::vector< DXGI_MODE_DESC > const & getDescs( VkFormat format )const
		{
			return m_descs[format];
		}

	private:
		std::vector< DXGI_MODE_DESC > m_displayModes;
		mutable std::map< VkFormat, std::vector< DXGI_MODE_DESC > > m_descs;
	};
}
