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
				auto context = device.getContext();
				context->glGetIntegerv( GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &value );
				return uint32_t( value );
			}();
			return result;
		}

		uint32_t doGetAlignedSize( uint32_t size, uint32_t align )
		{
			uint32_t result = 0u;
			auto align32 = uint32_t( align );

			while ( size > align )
			{
				size -= align32;
				result += align32;
			}

			return result + align32;
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
		return doGetAlignedSize( size, getOffsetAlignment( m_device ) );
	}
}
