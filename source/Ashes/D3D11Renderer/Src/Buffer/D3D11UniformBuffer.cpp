#include "Buffer/D3D11UniformBuffer.hpp"

#include "Core/D3D11Device.hpp"
#include "Core/D3D11PhysicalDevice.hpp"

namespace d3d11_renderer
{
	UniformBuffer::UniformBuffer( ashes::Device const & device
		, uint32_t count
		, uint32_t size
		, ashes::BufferTargets target
		, ashes::MemoryPropertyFlags flags )
		: ashes::UniformBufferBase{ device
			, count
			, size
			, target
			, flags }
	{
		m_buffer = m_device.createBuffer( count * getAlignedSize( getElementSize() )
			, target | ashes::BufferTarget::eUniformBuffer
			, flags );
	}

	uint32_t UniformBuffer::getAlignedSize( uint32_t size )const
	{
		return uint32_t( ashes::getAlignedSize( size
			, uint32_t( static_cast< Device const & >( m_device ).getPhysicalDevice().getProperties().limits.minUniformBufferOffsetAlignment ) ) );
	}
}
