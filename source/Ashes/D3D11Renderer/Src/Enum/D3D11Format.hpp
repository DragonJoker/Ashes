/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include <AshesPrerequisites.hpp>

namespace d3d11_renderer
{
	DXGI_FORMAT convert( ashes::Format const & format )noexcept;
	ashes::Format convert( DXGI_FORMAT const & format )noexcept;
}
