/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include <AshesPrerequisites.hpp>

namespace d3d11_renderer
{
	D3D11_MAP convert( ashes::MemoryMapFlags const & flags
		, ashes::MemoryPropertyFlags const & memory
		, ashes::BufferTargets const & targets );
	D3D11_MAP convert( ashes::MemoryMapFlags const & flags
		, ashes::MemoryPropertyFlags const & memory
		, ashes::ImageUsageFlags const & usage );
}
