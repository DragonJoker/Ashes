#include "Buffer/TestBuffer.hpp"

#include "Core/TestDevice.hpp"
#include "Miscellaneous/TestDeviceMemory.hpp"

#include <Ashes/Miscellaneous/MemoryRequirements.hpp>
#include <Ashes/Sync/BufferMemoryBarrier.hpp>

namespace test_renderer
{
	Buffer::Buffer( Device const & device
		, uint32_t size
		, VkBufferUsageFlags target )
		: ashes::BufferBase{ device
			, size
			, target }
	{
	}

	Buffer::~Buffer()
	{
	}

	ashes::MemoryRequirements Buffer::getMemoryRequirements()const
	{
		ashes::MemoryRequirements result{};
		result.alignment = 0u;
		result.memoryTypeBits = ~result.memoryTypeBits;
		result.size = getSize();
		result.type = ashes::ResourceType::eBuffer;
		return result;
	}

	void Buffer::doBindMemory()
	{
	}
}
