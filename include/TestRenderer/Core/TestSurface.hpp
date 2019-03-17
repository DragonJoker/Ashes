/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "TestRendererPrerequisites.hpp"

#include <Ashes/Core/Surface.hpp>

namespace test_renderer
{
	class Surface
		: public ashes::Surface
	{
	public:
		Surface( Instance const & instance
			, ashes::PhysicalDevice const & gpu
			, ashes::WindowHandle handle );

		bool getSupport( uint32_t queueFamilyIndex )const override;

		ashes::SurfaceCapabilities getCapabilities()const override
		{
			return m_surfaceCapabilities;
		}

		std::vector < ashes::PresentMode > getPresentModes()const override
		{
			return m_presentModes;
		}

		std::vector< ashes::SurfaceFormat > getFormats()const override
		{
			return m_surfaceFormats;
		}

	private:
		std::vector< ashes::SurfaceFormat > m_surfaceFormats;
		ashes::SurfaceCapabilities m_surfaceCapabilities;
		std::vector< ashes::PresentMode > m_presentModes;
	};
}
