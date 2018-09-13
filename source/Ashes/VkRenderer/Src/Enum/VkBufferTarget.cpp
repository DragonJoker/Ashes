#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkBufferUsageFlags convert( ashes::BufferTargets const & targets )
	{
		VkBufferUsageFlags result{ 0 };

		if ( checkFlag( targets, ashes::BufferTarget::eIndexBuffer ) )
		{
			result |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
		}

		if ( checkFlag( targets, ashes::BufferTarget::eDispatchIndirectBuffer )
			|| checkFlag( targets, ashes::BufferTarget::eDrawIndirectBuffer ) )
		{
			result |= VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT;
		}

		if ( checkFlag( targets, ashes::BufferTarget::eStorageBuffer ) )
		{
			result |= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
		}

		if ( checkFlag( targets, ashes::BufferTarget::eStorageTexelBuffer ) )
		{
			result |= VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT;
		}

		if ( checkFlag( targets, ashes::BufferTarget::eTransferDst )
			|| checkFlag( targets, ashes::BufferTarget::eUnpackBuffer ) )
		{
			result |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		}

		if ( checkFlag( targets, ashes::BufferTarget::eTransferSrc )
			|| checkFlag( targets, ashes::BufferTarget::ePackBuffer ) )
		{
			result |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		}

		if ( checkFlag( targets, ashes::BufferTarget::eUniformBuffer ) )
		{
			result |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
		}

		if ( checkFlag( targets, ashes::BufferTarget::eUniformTexelBuffer ) )
		{
			result |= VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT;
		}

		if ( checkFlag( targets, ashes::BufferTarget::eVertexBuffer ) )
		{
			result |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		}

		return result;
	}
}
