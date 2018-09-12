/*
This file belongs to RendererLib.
See LICENSE file in root folder
*/
#pragma once

#include <RendererPrerequisites.hpp>

namespace d3d11_renderer
{
	D3D11_COMPARISON_FUNC convert( renderer::CompareOp const & value );
}
