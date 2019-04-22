/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include <Ashes/Pipeline/StencilOpState.hpp>

namespace ashes::d3d11
{
	D3D11_DEPTH_STENCILOP_DESC convert( ashes::StencilOpState const & state );
}
