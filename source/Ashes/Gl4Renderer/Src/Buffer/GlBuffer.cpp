#include "Buffer/GlBuffer.hpp"

#include "Core/GlDevice.hpp"
#include "Miscellaneous/GlDeviceMemory.hpp"

#include <Miscellaneous/MemoryRequirements.hpp>
#include <Sync/BufferMemoryBarrier.hpp>

namespace gl_renderer
{
	Buffer::Buffer( Device const & device
		, uint32_t size
		, ashes::BufferTargets target )
		: ashes::BufferBase{ device
			, size
			, target }
		, m_device{ device }
		, m_target{ convert( target ) }
	{
		auto context = m_device.getContext();
		glLogCall( context
			, glGenBuffers
			, 1
			, &m_name );
	}

	Buffer::~Buffer()
	{
		onDestroy( m_name );
		m_storage.reset();
		auto context = m_device.getContext();
		glLogCall( context
			, glDeleteBuffers
			, 1
			, &m_name );
	}

	ashes::MemoryRequirements Buffer::getMemoryRequirements()const
	{
		ashes::MemoryRequirements result{};
		result.size = getSize();
		result.type = ashes::ResourceType::eBuffer;
		return result;
	}

	void Buffer::doBindMemory()
	{
		static_cast< DeviceMemory & >( *m_storage ).bindToBuffer( m_name, m_target );
	}
}
