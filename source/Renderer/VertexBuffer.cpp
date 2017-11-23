#include "VertexBuffer.hpp"

namespace renderer
{
	inline VertexBufferBase::VertexBufferBase( Device const & device
		, uint32_t bindingSlot
		, uint32_t size
		, BufferTargets target
		, MemoryPropertyFlags flags )
		: m_device{ device }
		, m_buffer{ device.getDevice().createVertexBuffer( size
			, convert( target )
			, convert( flags ) ) }
	{
	}
}
