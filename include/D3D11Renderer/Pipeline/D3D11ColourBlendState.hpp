/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include <Ashes/Pipeline/ColourBlendState.hpp>

namespace d3d11_renderer
{
	D3D11_BLEND_DESC convert( ashes::ColourBlendState const & state );
}
