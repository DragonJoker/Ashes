/*
This file belongs to VkRenderer.
See LICENSE file in root folder.
*/
#include "Buffer/VkGeometryBuffers.hpp"

namespace vk_renderer
{
	GeometryBuffers::GeometryBuffers( renderer::VertexBufferCRefArray const & vbos
		, std::vector< uint64_t > offsets
		, renderer::VertexInputState const & vertexInputState )
		: renderer::GeometryBuffers{ vbos, offsets, vertexInputState }
	{
	}

	GeometryBuffers::GeometryBuffers( renderer::VertexBufferCRefArray const & vbos
		, std::vector< uint64_t > offsets
		, renderer::VertexInputState const & vertexInputState
		, renderer::BufferBase const & ibo
		, uint64_t iboOffset
		, renderer::IndexType type )
		: renderer::GeometryBuffers{ vbos, offsets, vertexInputState, ibo, iboOffset, type }
	{
	}
}
