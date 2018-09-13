/*
This file belongs to RendererLib.
See LICENSE file in root folder
*/
#pragma once

#include <RendererPrerequisites.hpp>

namespace d3d11_renderer
{
	D3D11_MAP convert( renderer::MemoryMapFlags const & flags
		, renderer::MemoryPropertyFlags const & memory
		, renderer::BufferTargets const & targets );
	D3D11_MAP convert( renderer::MemoryMapFlags const & flags
		, renderer::MemoryPropertyFlags const & memory
		, renderer::ImageUsageFlags const & usage );
}
