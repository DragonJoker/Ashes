#include "BufferTarget.hpp"

namespace renderer
{
	VkBufferUsageFlags convert( BufferTargets const & targets )
	{
		VkBufferUsageFlags result{ 0 };

		if ( checkFlag( targets, BufferTarget::eIndexBuffer ) )
		{
			result |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
		}

		if ( checkFlag( targets, BufferTarget::eIndirectBuffer ) )
		{
			result |= VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT;
		}

		if ( checkFlag( targets, BufferTarget::eStorageBuffer ) )
		{
			result |= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
		}

		if ( checkFlag( targets, BufferTarget::eStorageTexelBuffer ) )
		{
			result |= VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT;
		}

		if ( checkFlag( targets, BufferTarget::eTransferDst ) )
		{
			result |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		}

		if ( checkFlag( targets, BufferTarget::eTransferSrc ) )
		{
			result |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		}

		if ( checkFlag( targets, BufferTarget::eUniformBuffer ) )
		{
			result |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
		}

		if ( checkFlag( targets, BufferTarget::eUniformTexelBuffer ) )
		{
			result |= VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT;
		}

		if ( checkFlag( targets, BufferTarget::eVertexBuffer ) )
		{
			result |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		}

		return result;
	}
}
