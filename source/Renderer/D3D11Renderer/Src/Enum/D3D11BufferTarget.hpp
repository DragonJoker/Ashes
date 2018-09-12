/*
This file belongs to RendererLib.
See LICENSE file in root folder
*/
#pragma once

#include <RendererPrerequisites.hpp>

namespace d3d11_renderer
{
	bool isPipelineBindable( renderer::BufferTargets const & targets );
	UINT convert( renderer::BufferTargets const & targets );
}
