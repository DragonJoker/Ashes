/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "AshesRenderer/Buffer/VertexBuffer.hpp"

namespace ashes
{
	VertexBufferBase::VertexBufferBase( Device const & device
		, uint32_t size
		, BufferTargets target )
		: m_device{ device }
		, m_size{ size }
		, m_buffer{ m_device.createBuffer( size
			, target | BufferTarget::eVertexBuffer ) }
	{
	}

	void VertexBufferBase::bindMemory( DeviceMemoryPtr memory )
	{
		m_buffer->bindMemory( std::move( memory ) );
	}

	MemoryRequirements VertexBufferBase::getMemoryRequirements()const
	{
		return m_buffer->getMemoryRequirements();
	}
}
