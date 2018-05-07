#include "Buffer/GlBuffer.hpp"

#include "Core/GlDevice.hpp"
#include "Miscellaneous/GlDeviceMemory.hpp"

#include <Miscellaneous/MemoryRequirements.hpp>
#include <Sync/BufferMemoryBarrier.hpp>

namespace gl_renderer
{
	Buffer::Buffer( renderer::Device const & device
		, uint32_t size
		, renderer::BufferTargets target )
		: renderer::BufferBase{ device
			, size
			, target }
		, m_target{ convert( target ) }
	{
		glLogCall( gl::GenBuffers, 1, &m_name );
	}

	Buffer::~Buffer()
	{
		onDestroy( m_name );
		m_storage.reset();
		glLogCall( gl::DeleteBuffers, 1, &m_name );
	}

	renderer::MemoryRequirements Buffer::getMemoryRequirements()const
	{
		renderer::MemoryRequirements result{};
		result.size = getSize();
		result.type = renderer::ResourceType::eBuffer;
		return result;
	}

	void Buffer::doBindMemory()
	{
		static_cast< DeviceMemory & >( *m_storage ).bindToBuffer( m_name, m_target );
	}
}
