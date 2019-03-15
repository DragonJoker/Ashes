/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Core/GlConnection.hpp"

#include <Ashes/Core/PlatformWindowHandle.hpp>

namespace gl_renderer
{
	Connection::Connection( ashes::Instance const & instance
		, ashes::PhysicalDevice const & gpu
		, ashes::WindowHandle handle )
		: ashes::Connection{ instance
			, gpu
			, std::move( handle ) }
	{
		m_surfaceType = "OpenGL surface";
	}

	bool Connection::getSurfaceSupport( uint32_t queueFamilyIndex )const
	{
		return true;
	}
}
