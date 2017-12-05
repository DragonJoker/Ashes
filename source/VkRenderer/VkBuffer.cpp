#include "VkBuffer.hpp"

#include "VkDevice.hpp"

namespace vk_renderer
{
	BufferBase::BufferBase( renderer::Device const & device
		, uint32_t size
		, renderer::BufferTargets target
		, renderer::MemoryPropertyFlags flags )
		: renderer::BufferBase{ device
			, size
			, target
			, flags }
		, m_buffer{ static_cast< Device const & >( device ).getDevice()
			, size
			, convert( target )
			, convert( flags ) }
	{
	}
}
