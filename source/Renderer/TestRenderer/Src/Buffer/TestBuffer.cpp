#include "Buffer/TestBuffer.hpp"

#include "Core/TestDevice.hpp"
#include "Miscellaneous/TestDeviceMemory.hpp"

#include <Miscellaneous/MemoryRequirements.hpp>
#include <Sync/BufferMemoryBarrier.hpp>

namespace test_renderer
{
	Buffer::Buffer( Device const & device
		, uint32_t size
		, renderer::BufferTargets target )
		: renderer::BufferBase{ device
			, size
			, target }
	{
	}

	Buffer::~Buffer()
	{
	}

	renderer::MemoryRequirements Buffer::getMemoryRequirements()const
	{
		renderer::MemoryRequirements result;
		result.alignment = 0u;
		result.memoryTypeBits = 0xFFFFFFFF;
		result.size = getSize();
		result.type = renderer::ResourceType::eBuffer;
		return result;
	}

	void Buffer::doBindMemory()
	{
	}
}
