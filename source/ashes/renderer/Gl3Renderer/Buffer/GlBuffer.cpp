#include "Buffer/GlBuffer.hpp"

#include "Core/GlContextLock.hpp"
#include "Core/GlDevice.hpp"
#include "Miscellaneous/GlDeviceMemory.hpp"

#include "ashesgl3_api.hpp"

namespace ashes::gl3
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
		m_copyTarget = GlBufferTarget( 0u );
		m_memory = VK_NULL_HANDLE;
		m_target = GlBufferTarget( 0u );
		m_internal = GL_INVALID_INDEX;
		m_queueFamilyIndices.clear();
	}

	VkMemoryRequirements Buffer::getMemoryRequirements()const
	{
		VkMemoryRequirements result{};
		result.size = m_createInfo.size;
		result.memoryTypeBits = ~( 0u );
		result.alignment = get( m_device )->getLimits().nonCoherentAtomSize;

		if ( checkFlag( m_createInfo.usage, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT ) )
		{
			result.alignment = get( m_device )->getLimits().minUniformBufferOffsetAlignment;
		}

		result.size = ashes::getAlignedSize( m_createInfo.size, result.alignment );
		return result;
	}

	bool Buffer::isMapped()const
	{
		assert( m_memory != VK_NULL_HANDLE );
		return get( m_memory )->isMapped();
	}
}
