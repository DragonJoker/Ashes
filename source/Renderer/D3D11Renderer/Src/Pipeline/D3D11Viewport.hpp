/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#pragma once

#include <Pipeline/Viewport.hpp>

namespace d3d11_renderer
{
	D3D11_VIEWPORT makeViewport( renderer::Viewport const & viewport );
}
