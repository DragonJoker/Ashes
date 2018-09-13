/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#pragma once

#include <Pipeline/Scissor.hpp>

namespace d3d11_renderer
{
	RECT makeScissor( renderer::Scissor const & scissor );
	RECT makeScissor( renderer::Extent2D const & extent );
}
