/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include <Ashes/Pipeline/Viewport.hpp>

namespace ashes::d3d11
{
	D3D11_VIEWPORT makeViewport( ashes::Viewport const & viewport );
	std::vector< D3D11_VIEWPORT > makeViewports( ashes::VkViewportArray::const_iterator const & begin
		, ashes::VkViewportArray::const_iterator const & end );
}
