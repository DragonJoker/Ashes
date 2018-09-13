#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkAccessFlags convert( ashes::AccessFlags const & flags )
	{
		VkAccessFlags result{ 0 };

		if ( checkFlag( flags, ashes::AccessFlag::eIndirectCommandRead ) )
		{
			result |= VK_ACCESS_INDIRECT_COMMAND_READ_BIT;
		}

		if ( checkFlag( flags, ashes::AccessFlag::eIndexRead ) )
		{
			result |= VK_ACCESS_INDEX_READ_BIT;
		}

		if ( checkFlag( flags, ashes::AccessFlag::eVertexAttributeRead ) )
		{
			result |= VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT;
		}

		if ( checkFlag( flags, ashes::AccessFlag::eUniformRead ) )
		{
			result |= VK_ACCESS_UNIFORM_READ_BIT;
		}

		if ( checkFlag( flags, ashes::AccessFlag::eInputAttachmentRead ) )
		{
			result |= VK_ACCESS_INPUT_ATTACHMENT_READ_BIT;
		}

		if ( checkFlag( flags, ashes::AccessFlag::eShaderRead ) )
		{
			result |= VK_ACCESS_SHADER_READ_BIT;
		}

		if ( checkFlag( flags, ashes::AccessFlag::eShaderWrite ) )
		{
			result |= VK_ACCESS_SHADER_WRITE_BIT;
		}

		if ( checkFlag( flags, ashes::AccessFlag::eColourAttachmentRead ) )
		{
			result |= VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
		}

		if ( checkFlag( flags, ashes::AccessFlag::eColourAttachmentWrite ) )
		{
			result |= VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		}

		if ( checkFlag( flags, ashes::AccessFlag::eDepthStencilAttachmentRead ) )
		{
			result |= VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
		}

		if ( checkFlag( flags, ashes::AccessFlag::eDepthStencilAttachmentWrite ) )
		{
			result |= VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
		}

		if ( checkFlag( flags, ashes::AccessFlag::eTransferRead ) )
		{
			result |= VK_ACCESS_TRANSFER_READ_BIT;
		}

		if ( checkFlag( flags, ashes::AccessFlag::eTransferWrite ) )
		{
			result |= VK_ACCESS_TRANSFER_WRITE_BIT;
		}

		if ( checkFlag( flags, ashes::AccessFlag::eHostRead ) )
		{
			result |= VK_ACCESS_HOST_READ_BIT;
		}

		if ( checkFlag( flags, ashes::AccessFlag::eHostWrite ) )
		{
			result |= VK_ACCESS_HOST_WRITE_BIT;
		}

		if ( checkFlag( flags, ashes::AccessFlag::eMemoryRead ) )
		{
			result |= VK_ACCESS_MEMORY_READ_BIT;
		}

		if ( checkFlag( flags, ashes::AccessFlag::eMemoryWrite ) )
		{
			result |= VK_ACCESS_MEMORY_WRITE_BIT;
		}

		return result;
	}

	ashes::AccessFlags convertAccessFlags( VkAccessFlags const & flags )
	{
		ashes::AccessFlags result;

		if ( ashes::checkFlag( flags, VK_ACCESS_INDIRECT_COMMAND_READ_BIT ) )
		{
			result |= ashes::AccessFlag::eIndirectCommandRead;
		}

		if ( ashes::checkFlag( flags, VK_ACCESS_INDEX_READ_BIT ) )
		{
			result |= ashes::AccessFlag::eIndexRead;
		}

		if ( ashes::checkFlag( flags, VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT ) )
		{
			result |= ashes::AccessFlag::eVertexAttributeRead;
		}

		if ( ashes::checkFlag( flags, VK_ACCESS_UNIFORM_READ_BIT ) )
		{
			result |= ashes::AccessFlag::eUniformRead;
		}

		if ( ashes::checkFlag( flags, VK_ACCESS_INPUT_ATTACHMENT_READ_BIT ) )
		{
			result |= ashes::AccessFlag::eInputAttachmentRead;
		}

		if ( ashes::checkFlag( flags, VK_ACCESS_SHADER_READ_BIT ) )
		{
			result |= ashes::AccessFlag::eShaderRead;
		}

		if ( ashes::checkFlag( flags, VK_ACCESS_SHADER_WRITE_BIT ) )
		{
			result |= ashes::AccessFlag::eShaderWrite;
		}

		if ( ashes::checkFlag( flags, VK_ACCESS_COLOR_ATTACHMENT_READ_BIT ) )
		{
			result |= ashes::AccessFlag::eColourAttachmentRead;
		}

		if ( ashes::checkFlag( flags, VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT ) )
		{
			result |= ashes::AccessFlag::eColourAttachmentWrite;
		}

		if ( ashes::checkFlag( flags, VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT ) )
		{
			result |= ashes::AccessFlag::eDepthStencilAttachmentRead;
		}

		if ( ashes::checkFlag( flags, VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT ) )
		{
			result |= ashes::AccessFlag::eDepthStencilAttachmentWrite;
		}

		if ( ashes::checkFlag( flags, VK_ACCESS_TRANSFER_READ_BIT ) )
		{
			result |= ashes::AccessFlag::eTransferRead;
		}

		if ( ashes::checkFlag( flags, VK_ACCESS_TRANSFER_WRITE_BIT ) )
		{
			result |= ashes::AccessFlag::eTransferWrite;
		}

		if ( ashes::checkFlag( flags, VK_ACCESS_HOST_READ_BIT ) )
		{
			result |= ashes::AccessFlag::eHostRead;
		}

		if ( ashes::checkFlag( flags, VK_ACCESS_HOST_WRITE_BIT ) )
		{
			result |= ashes::AccessFlag::eHostWrite;
		}

		if ( ashes::checkFlag( flags, VK_ACCESS_MEMORY_READ_BIT ) )
		{
			result |= ashes::AccessFlag::eMemoryRead;
		}

		if ( ashes::checkFlag( flags, VK_ACCESS_MEMORY_WRITE_BIT ) )
		{
			result |= ashes::AccessFlag::eMemoryWrite;
		}

		return result;
	}
}
