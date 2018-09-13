/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#pragma once

#include <RendererPrerequisites.hpp>

namespace d3d11_renderer
{
	DXGI_FORMAT convert( renderer::Format const & format )noexcept;
	renderer::Format convert( DXGI_FORMAT const & format )noexcept;
}
