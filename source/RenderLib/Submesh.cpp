#include "Submesh.h"

#include "Mesh.h"

namespace render
{
	Submesh::Submesh( renderer::RenderingResources const & resources
		, Mesh const & mesh
		, UInt16Array const & idx )
		: m_mesh{ mesh }
		, m_index{ renderer::makeBuffer< uint16_t >( resources.getDevice()
			, uint32_t( idx.size() )
			, renderer::BufferTarget::eIndexBuffer | renderer::BufferTarget::eTransferDst
			, renderer::MemoryPropertyFlag::eDeviceLocal ) }
	{
		resources.getStagingBuffer().copyBufferData( resources.getCommandBuffer()
			, idx
			, *m_index );
	}

	uint32_t Submesh::getIndexCount()const noexcept
	{
		return m_index->getCount();
	}

	renderer::Buffer< uint16_t > const & Submesh::getIbo()const noexcept
	{
		return *m_index;
	}
}
