/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include <AshesPrerequisites.hpp>

namespace d3d11_renderer
{
	bool isRenderTarget( ashes::ImageUsageFlags const & flags );
	bool isStorage( ashes::ImageUsageFlags const & flags );
	bool isSampled( ashes::ImageUsageFlags const & flags );
}
