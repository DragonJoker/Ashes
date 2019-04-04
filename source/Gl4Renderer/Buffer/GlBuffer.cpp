#include "Buffer/GlBuffer.hpp"

#include "Core/GlDevice.hpp"
#include "Miscellaneous/GlDeviceMemory.hpp"

#include "ashesgl4_api.hpp"

namespace ashes::gl4
{
	Buffer::Buffer( VkDevice device
		, VkBufferCreateInfo createInfo )
		: m_device{ device }
		, m_flags{ createInfo.flags }
		, m_size{ createInfo.size }
		, m_usage{ createInfo.usage }
		, m_sharingMode{ createInfo.sharingMode }
		, m_queueFamilyIndices{ makeVector( createInfo.pQueueFamilyIndices, createInfo.queueFamilyIndexCount ) }
		, m_target{ getTargetFromUsageFlags( m_usage ) }
	{
		auto context = get( m_device )->getContext();
		glLogCall( context
			, glGenBuffers
			, 1
			, &m_name );
	}

	Buffer::~Buffer()
	{
		onDestroy( m_name );
		auto context = get( m_device )->getContext();
		glLogCall( context
			, glDeleteBuffers
			, 1
			, &m_name );
	}

	VkMemoryRequirements Buffer::getMemoryRequirements()const
	{
		VkMemoryRequirements result{};
		result.size = m_size;
		result.alignment = 1u;
		result.memoryTypeBits = ~( 0u );
		return result;
	}
}
