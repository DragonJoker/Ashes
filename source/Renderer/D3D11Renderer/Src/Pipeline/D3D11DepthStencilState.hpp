/*
This file belongs to RendererLib.
See LICENSE file in root folder
*/
#pragma once

#include <Pipeline/DepthStencilState.hpp>

namespace d3d11_renderer
{
	D3D11_DEPTH_STENCIL_DESC convert( renderer::DepthStencilState const & state );
}
