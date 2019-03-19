/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Core/GlSurface.hpp"

#include <Ashes/Core/PlatformWindowHandle.hpp>
#include <PlatformUtils/SurfaceInfos.hpp>

namespace gl_renderer
{
	Surface::Surface( ashes::Instance const & instance
		, ashes::PhysicalDevice const & gpu
		, ashes::WindowHandle handle )
		: ashes::Surface{ instance
			, gpu
			, std::move( handle ) }
	{
		m_presentModes.push_back( ashes::PresentMode::eFifo );
		utils::getSurfaceInfos( m_handle, m_type, m_surfaceFormats, m_surfaceCapabilities );
	}

	bool Surface::getSupport( uint32_t queueFamilyIndex )const
	{
		return true;
	}
}
