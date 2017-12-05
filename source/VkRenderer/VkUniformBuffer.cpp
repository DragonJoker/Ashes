#include "VkUniformBuffer.hpp"

#include "VkDevice.hpp"

#include <VkLib/LogicalDevice.hpp>

namespace vk_renderer
{
	UniformBufferBase::UniformBufferBase( renderer::Device const & device
		, uint32_t count
		, uint32_t size
		, renderer::BufferTargets target
		, renderer::MemoryPropertyFlags flags )
		: renderer::UniformBufferBase{ device
			, count
			, vk::UniformBuffer::getOffset( static_cast< Device const & >( device ).getDevice()
				, 1u
				, size )
			, target
			, flags }
	{
	}
}
