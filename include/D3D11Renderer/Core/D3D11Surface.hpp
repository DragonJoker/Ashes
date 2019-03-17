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
	};
}
