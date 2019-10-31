#include "Buffer/TestBuffer.hpp"

#include "Core/TestDevice.hpp"
#include "Miscellaneous/TestDeviceMemory.hpp"

#include "ashestest_api.hpp"

namespace ashes::test
{
	Buffer::Buffer( VkDevice device
		, VkBufferCreateInfo createInfo )
		: m_device{ device }
		, m_createInfo{ std::move( createInfo ) }
	{
	}

	Buffer::~Buffer()
	{
	}

	VkMemoryRequirements Buffer::getMemoryRequirements()const
	{
		VkMemoryRequirements result{};
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

	void Buffer::copyFrom( VkBuffer src
		, VkDeviceSize srcOffset
		, VkDeviceSize srcSize
		, VkDeviceSize dstOffset )const
	{
		get( m_memory )->updateData( get( src )->m_memory
			, srcOffset
			, dstOffset
			, srcSize );
	}

	VkResult Buffer::bindMemory( VkDeviceMemory memory
		, VkDeviceSize memoryOffset )
	{
		m_memory = memory;
		m_memoryOffset = memoryOffset;
		auto result = get( m_memory )->bindToBuffer( get( this )
			, m_memoryOffset
			, m_objectMemory );
		return result;
	}
}
