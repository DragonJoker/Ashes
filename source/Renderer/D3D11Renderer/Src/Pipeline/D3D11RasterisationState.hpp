/*
This file belongs to RendererLib.
See LICENSE file in root folder
*/
#pragma once

#include <Pipeline/RasterisationState.hpp>

namespace d3d11_renderer
{
	D3D11_RASTERIZER_DESC convert( renderer::RasterisationState const & state
		, renderer::MultisampleState const & ms );
}
