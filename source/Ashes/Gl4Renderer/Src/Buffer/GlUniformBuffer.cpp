#include "Buffer/GlUniformBuffer.hpp"

#include "Core/GlDevice.hpp"

namespace gl_renderer
{
	namespace
	{
		uint32_t getOffsetAlignment( Device const & device )
		{
			static uint32_t const result = [&device]()
			{
				GLint value;
				device.getContext()->glGetIntegerv( GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &value );
				return uint32_t( value );
			}();
			return result;
		}
	}

	UniformBuffer::UniformBuffer( Device const & device
		, uint32_t count
		, uint32_t size
		, ashes::BufferTargets target
		, ashes::MemoryPropertyFlags flags )
		: ashes::UniformBufferBase{ device
			, count
			, size
			, target
			, flags }
		, m_device{ device }
	{
		m_buffer = static_cast< ashes::Device const & >( m_device ).createBuffer( count * getAlignedSize( getElementSize() )
			, target | ashes::BufferTarget::eUniformBuffer
			, flags );
	}

	uint32_t UniformBuffer::getAlignedSize( uint32_t size )const
	{
		return uint32_t( ashes::getAlignedSize( size
			, getOffsetAlignment( m_device ) ) );
	}
}
