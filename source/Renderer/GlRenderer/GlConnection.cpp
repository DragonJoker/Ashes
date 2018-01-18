/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#include "GlConnection.hpp"

#include "GlRenderer.hpp"

#include <Renderer/Core/PlatformWindowHandle.hpp>

namespace gl_renderer
{
	Connection::Connection( renderer::Renderer const & renderer
		, uint32_t deviceIndex
		, renderer::WindowHandle && handle )
		: renderer::Connection{ renderer
			, deviceIndex
			, std::move( handle ) }
	{
	}
}
