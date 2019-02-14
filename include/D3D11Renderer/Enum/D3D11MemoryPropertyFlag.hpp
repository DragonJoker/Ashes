/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include <Ashes/AshesPrerequisites.hpp>

namespace d3d11_renderer
{
	bool isHostVisible( ashes::MemoryPropertyFlags const & flags );
	// Buffer
	D3D11_USAGE getUsage( ashes::MemoryPropertyFlags const & flags
		, ashes::BufferTargets const & targets );
	D3D11_USAGE getUsage( ashes::MemoryPropertyFlags const & flags
		, ashes::ImageUsageFlags const & usage );
	// Image
	UINT getCpuAccessFlags( ashes::MemoryPropertyFlags const & flags
		, ashes::BufferTargets const & targets );
	UINT getCpuAccessFlags( ashes::MemoryPropertyFlags const & flags
		, ashes::ImageUsageFlags const & usage );
}
