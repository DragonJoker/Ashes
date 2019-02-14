/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include <Ashes/Pipeline/Scissor.hpp>

namespace d3d11_renderer
{
	RECT makeScissor( ashes::Scissor const & scissor );
	std::vector< RECT > makeScissors( ashes::ScissorArray::const_iterator const & begin
		, ashes::ScissorArray::const_iterator const & end );
	RECT makeScissor( ashes::Extent2D const & extent );
}
