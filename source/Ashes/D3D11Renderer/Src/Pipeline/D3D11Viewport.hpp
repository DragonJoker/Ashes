/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include <Pipeline/Viewport.hpp>

namespace d3d11_renderer
{
	D3D11_VIEWPORT makeViewport( ashes::Viewport const & viewport );
	std::vector< D3D11_VIEWPORT > makeViewports( ashes::ViewportArray::const_iterator const & begin
		, ashes::ViewportArray::const_iterator const & end );
}
