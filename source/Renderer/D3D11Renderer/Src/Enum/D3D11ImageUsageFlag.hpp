/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#pragma once

#include <RendererPrerequisites.hpp>

namespace d3d11_renderer
{
	bool isRenderTarget( renderer::ImageUsageFlags const & flags );
	bool isStorage( renderer::ImageUsageFlags const & flags );
}
