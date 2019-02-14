/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include <Ashes/Pipeline/DepthStencilState.hpp>

namespace d3d11_renderer
{
	D3D11_DEPTH_STENCIL_DESC convert( ashes::DepthStencilState const & state );
}
