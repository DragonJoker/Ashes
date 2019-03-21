/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Ashes/Buffer/UniformBuffer.hpp"

#include "Ashes/Core/Device.hpp"
#include "Ashes/Miscellaneous/MemoryRequirements.hpp"

namespace ashes
{
	UniformBufferBase::UniformBufferBase( Device const & device
		, uint32_t count
		, uint32_t size
		, BufferTargets target )
		: m_device{ device }
		, m_count{ count }
		, m_size{ size }
		, m_buffer{ m_device.createBuffer( count * getAlignedSize( getElementSize() )
			, target | ashes::BufferTarget::eUniformBuffer ) }
	{
	}

	UniformBufferBase::~UniformBufferBase()
	{
	}

	void UniformBufferBase::bindMemory( DeviceMemoryPtr memory )
	{
		m_buffer->bindMemory( std::move( memory ) );
	}

	MemoryRequirements UniformBufferBase::getMemoryRequirements()const
	{
		return m_buffer->getMemoryRequirements();
	}

	uint32_t UniformBufferBase::getAlignedSize( uint32_t size )const
	{
		return uint32_t( ashes::getAlignedSize( size
			, uint32_t( m_device.getProperties().limits.minUniformBufferOffsetAlignment ) ) );
	}
}
