#include "Buffer/GlBuffer.hpp"

#include "Core/GlDevice.hpp"
#include "Miscellaneous/GlDeviceMemory.hpp"

#include <Ashes/Miscellaneous/MemoryRequirements.hpp>
#include <Ashes/Sync/BufferMemoryBarrier.hpp>

namespace gl_renderer
{
	Buffer::Buffer( Device const & device
		, VkBufferCreateInfo createInfo )
		: m_device{ device }
		, m_createInfo{ createInfo }
		, m_target{ getTargetFromUsageFlags( m_createInfo.usage ) }
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

	VkMemoryRequirements Buffer::getMemoryRequirements()const
	{
		VkMemoryRequirements result{};
		result.size = m_createInfo.size;
		result.alignment = 1u;
		result.memoryTypeBits = ~result.memoryTypeBits;
		return result;
	}

	void Buffer::bindMemory( DeviceMemoryPtr memory )const
	{
		assert( !m_storage && "A resource can only be bound once to a device memory object." );
		m_storage = std::move( memory );
		static_cast< DeviceMemory & >( *m_storage ).bindToBuffer( m_name, m_target );
	}
}
