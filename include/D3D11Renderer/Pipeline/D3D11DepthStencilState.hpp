/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include <Ashes/Pipeline/DepthStencilState.hpp>

namespace ashes::d3d11
{
	D3D11_DEPTH_STENCIL_DESC convert( ashes::DepthStencilState const & state );
}
