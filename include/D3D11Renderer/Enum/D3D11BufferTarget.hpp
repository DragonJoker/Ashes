/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include <Ashes/AshesPrerequisites.hpp>

namespace d3d11_renderer
{
	bool isPipelineBindable( ashes::BufferTargets const & targets );
	UINT convert( ashes::BufferTargets const & targets );
}
