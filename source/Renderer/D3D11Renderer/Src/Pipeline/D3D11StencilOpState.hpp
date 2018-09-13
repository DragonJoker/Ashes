/*
This file belongs to RendererLib.
See LICENSE file in root folder
*/
#pragma once

#include <Pipeline/StencilOpState.hpp>

namespace d3d11_renderer
{
	D3D11_DEPTH_STENCILOP_DESC convert( renderer::StencilOpState const & state );
}
