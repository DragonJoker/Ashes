/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include <Ashes/Pipeline/RasterisationState.hpp>

namespace ashes::d3d11
{
	D3D11_RASTERIZER_DESC convert( ashes::RasterisationState const & state
		, ashes::MultisampleState const & ms );
}
