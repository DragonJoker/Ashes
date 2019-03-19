/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "D3D11Renderer/D3D11RendererPrerequisites.hpp"

#include <Ashes/Core/Surface.hpp>

namespace d3d11_renderer
{
	class Surface
		: public ashes::Surface
	{
	public:
		Surface( Instance const & instance
			, ashes::PhysicalDevice const & gpu
			, ashes::WindowHandle handle );
		~Surface();

		bool getSupport( uint32_t queueFamilyIndex )const override;
		std::vector < ashes::PresentMode > getPresentModes()const override;
		ashes::SurfaceCapabilities getCapabilities()const override;
		std::vector< ashes::SurfaceFormat > getFormats()const override;

		inline std::vector< DXGI_MODE_DESC > const & getDescs( ashes::Format format )const
		{
			return m_descs[format];
		}

	private:
		std::vector< DXGI_MODE_DESC > m_displayModes;
		mutable std::map< ashes::Format, std::vector< DXGI_MODE_DESC > > m_descs;
	};
}
