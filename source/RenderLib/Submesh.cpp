#include "Submesh.h"

#include "Mesh.h"

namespace render
{
	Submesh::Submesh( renderer::Device const & device
		, renderer::StagingBuffer const & stagingBuffer
		, renderer::CommandBuffer const & commandBuffer
		, Mesh const & mesh
		, UInt16Array const & idx )
		: m_mesh{ mesh }
		, m_index{ renderer::makeBuffer< uint16_t >( device
			, uint32_t( idx.size() )
			, renderer::BufferTarget::eIndexBuffer | renderer::BufferTarget::eTransferDst
			, renderer::MemoryPropertyFlag::eDeviceLocal ) }
	{
		stagingBuffer.copyBufferData( commandBuffer
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
