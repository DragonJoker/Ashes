/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include <Pipeline/RasterisationState.hpp>

namespace d3d11_renderer
{
	D3D11_RASTERIZER_DESC convert( ashes::RasterisationState const & state
		, ashes::MultisampleState const & ms );
}
