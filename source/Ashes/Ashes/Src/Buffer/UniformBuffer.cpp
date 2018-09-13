/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Buffer/UniformBuffer.hpp"

#include "Core/Device.hpp"

namespace ashes
{
	UniformBufferBase::UniformBufferBase( Device const & device
		, uint32_t count
		, uint32_t size
		, BufferTargets target
		, MemoryPropertyFlags flags )
		: m_device{ device }
		, m_count{ count }
		, m_size{ size }
	{
		registerObject( m_device, "UniformBuffer", this );
	}

	UniformBufferBase::~UniformBufferBase()
	{
		unregisterObject( m_device, this );
	}
}
