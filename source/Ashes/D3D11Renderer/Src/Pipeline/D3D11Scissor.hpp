/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include <Pipeline/Scissor.hpp>

namespace d3d11_renderer
{
	RECT makeScissor( ashes::Scissor const & scissor );
	RECT makeScissor( ashes::Extent2D const & extent );
}
