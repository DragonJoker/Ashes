/*
This file belongs to RendererLib.
See LICENSE file in root folder
*/
#pragma once

#include <RendererPrerequisites.hpp>

namespace d3d11_renderer
{
	bool isHostVisible( renderer::MemoryPropertyFlags const & flags );
	// Buffer
	D3D11_USAGE getUsage( renderer::MemoryPropertyFlags const & flags
		, renderer::BufferTargets const & targets );
	D3D11_USAGE getUsage( renderer::MemoryPropertyFlags const & flags
		, renderer::ImageUsageFlags const & usage );
	// Image
	UINT getCpuAccessFlags( renderer::MemoryPropertyFlags const & flags
		, renderer::BufferTargets const & targets );
	UINT getCpuAccessFlags( renderer::MemoryPropertyFlags const & flags
		, renderer::ImageUsageFlags const & usage );
}
