#include "Buffer/GlBuffer.hpp"

#include "Core/GlContextLock.hpp"
#include "Core/GlDevice.hpp"
#include "Miscellaneous/GlDeviceMemory.hpp"

#include "ashesgl4_api.hpp"

namespace ashes
{
	inline uint32_t deepCopy( uint32_t const & rhs )
	{
		return rhs;
	}
}
	
namespace ashes::gl4
{
	Buffer::Buffer( VkDevice device
		, VkBufferCreateInfo createInfo )
		: m_device{ device }
		, m_queueFamilyIndices{ makeVector( createInfo.pQueueFamilyIndices, createInfo.queueFamilyIndexCount ) }
		, m_createInfo{ createInfo }
		, m_target{ getTargetFromUsageFlags( m_createInfo.usage ) }
	{
		m_createInfo.pQueueFamilyIndices = m_queueFamilyIndices.data();
	}

	Buffer::~Buffer()
	{
		onDestroy( m_internal );
	}

	VkMemoryRequirements Buffer::getMemoryRequirements()const
	{
		VkMemoryRequirements result{};
		result.size = m_createInfo.size;
		result.alignment = 1u;
		result.memoryTypeBits = ~( 0u );
		return result;
	}

	bool Buffer::isMapped()const
	{
		assert( m_memory != VK_NULL_HANDLE );
		return get( m_memory )->isMapped();
	}
}
