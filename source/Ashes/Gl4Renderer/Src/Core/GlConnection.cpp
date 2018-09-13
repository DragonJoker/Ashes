/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Core/GlConnection.hpp"

#include "Core/GlRenderer.hpp"

#include <Core/PlatformWindowHandle.hpp>

namespace gl_renderer
{
	Connection::Connection( ashes::Renderer const & renderer
		, uint32_t deviceIndex
		, ashes::WindowHandle && handle )
		: ashes::Connection{ renderer
			, deviceIndex
			, std::move( handle ) }
	{
		m_surfaceType = "OpenGL surface";
	}
}
