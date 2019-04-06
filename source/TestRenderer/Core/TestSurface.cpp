/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Core/TestSurface.hpp"

#include "Core/TestPhysicalDevice.hpp"
#include "Core/TestInstance.hpp"

#include <Ashes/Core/PlatformWindowHandle.hpp>
#include <PlatformUtils/SurfaceInfos.hpp>

namespace test_renderer
{
	Surface::Surface( Instance const & instance
		, ashes::PhysicalDevice const & gpu
		, ashes::WindowHandle handle )
		: ashes::Surface{ instance, gpu, std::move( handle ) }
	{
		m_presentModes.push_back( VK_PRESENT_MODE_FIFO_KHR );
		utils::getSurfaceInfos( m_handle, m_type, m_surfaceFormats, m_surfaceCapabilities );
	}

	bool Surface::getSupport( uint32_t queueFamilyIndex )const
	{
		return true;
	}
}
