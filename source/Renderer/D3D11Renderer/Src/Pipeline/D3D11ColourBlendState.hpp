/*
This file belongs to RendererLib.
See LICENSE file in root folder
*/
#pragma once

#include <Pipeline/ColourBlendState.hpp>

namespace d3d11_renderer
{
	D3D11_BLEND_DESC convert( renderer::ColourBlendState const & state );
}
