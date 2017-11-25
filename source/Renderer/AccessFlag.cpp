#include "AccessFlag.hpp"

namespace renderer
{
	VkAccessFlags convert( AccessFlags const & flags )
	{
		VkAccessFlags result{ 0 };

		if ( checkFlag( flags, AccessFlag::eIndirectCommandRead ) )
		{
			result |= VK_ACCESS_INDIRECT_COMMAND_READ_BIT;
		}

		if ( checkFlag( flags, AccessFlag::eIndexRead ) )
		{
			result |= VK_ACCESS_INDEX_READ_BIT;
		}

		if ( checkFlag( flags, AccessFlag::eVertexAttributeRead ) )
		{
			result |= VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT;
		}

		if ( checkFlag( flags, AccessFlag::eUniformRead ) )
		{
			result |= VK_ACCESS_UNIFORM_READ_BIT;
		}

		if ( checkFlag( flags, AccessFlag::eInputAttachmentRead ) )
		{
			result |= VK_ACCESS_INPUT_ATTACHMENT_READ_BIT;
		}

		if ( checkFlag( flags, AccessFlag::eShaderRead ) )
		{
			result |= VK_ACCESS_SHADER_READ_BIT;
		}

		if ( checkFlag( flags, AccessFlag::eShaderWrite ) )
		{
			result |= VK_ACCESS_SHADER_WRITE_BIT;
		}

		if ( checkFlag( flags, AccessFlag::eColourAttachmentRead ) )
		{
			result |= VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
		}

		if ( checkFlag( flags, AccessFlag::eColourAttachmentWrite ) )
		{
			result |= VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		}

		if ( checkFlag( flags, AccessFlag::eDepthStencilAttachmentRead ) )
		{
			result |= VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
		}

		if ( checkFlag( flags, AccessFlag::eDepthStencilAttachmentWrite ) )
		{
			result |= VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
		}

		if ( checkFlag( flags, AccessFlag::eTransferRead ) )
		{
			result |= VK_ACCESS_TRANSFER_READ_BIT;
		}

		if ( checkFlag( flags, AccessFlag::eTransferWrite ) )
		{
			result |= VK_ACCESS_TRANSFER_WRITE_BIT;
		}

		if ( checkFlag( flags, AccessFlag::eHostRead ) )
		{
			result |= VK_ACCESS_HOST_READ_BIT;
		}

		if ( checkFlag( flags, AccessFlag::eHostWrite ) )
		{
			result |= VK_ACCESS_HOST_WRITE_BIT;
		}

		if ( checkFlag( flags, AccessFlag::eMemoryRead ) )
		{
			result |= VK_ACCESS_MEMORY_READ_BIT;
		}

		if ( checkFlag( flags, AccessFlag::eMemoryWrite ) )
		{
			result |= VK_ACCESS_MEMORY_WRITE_BIT;
		}

		return result;
	}
}
