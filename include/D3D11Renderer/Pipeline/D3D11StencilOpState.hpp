/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include <Ashes/Pipeline/StencilOpState.hpp>

namespace d3d11_renderer
{
	D3D11_DEPTH_STENCILOP_DESC convert( ashes::StencilOpState const & state );
}
