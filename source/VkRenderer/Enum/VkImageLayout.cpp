#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkImageLayout convert( ashes::ImageLayout const & layout )
	{
		switch ( layout )
		{
		case ashes::ImageLayout::eUndefined:
			return VK_IMAGE_LAYOUT_UNDEFINED;

		case ashes::ImageLayout::eGeneral:
			return VK_IMAGE_LAYOUT_GENERAL;

		case ashes::ImageLayout::eColourAttachmentOptimal:
			return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		case ashes::ImageLayout::eDepthStencilAttachmentOptimal:
			return VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		case ashes::ImageLayout::eDepthStencilReadOnlyOptimal:
			return VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;

		case ashes::ImageLayout::eShaderReadOnlyOptimal:
			return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

		case ashes::ImageLayout::eTransferSrcOptimal:
			return VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;

		case ashes::ImageLayout::eTransferDstOptimal:
			return VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;

		case ashes::ImageLayout::ePreinitialised:
			return VK_IMAGE_LAYOUT_PREINITIALIZED;

		case ashes::ImageLayout::ePresentSrc:
			return VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		default:
			assert( false && "Unsupported image layout" );
			return VK_IMAGE_LAYOUT_UNDEFINED;
		}
	}

	ashes::ImageLayout convertImageLayout( VkImageLayout const & layout )
	{
		switch ( layout )
		{
		case VK_IMAGE_LAYOUT_UNDEFINED:
			return ashes::ImageLayout::eUndefined;

		case VK_IMAGE_LAYOUT_GENERAL:
			return ashes::ImageLayout::eGeneral;

		case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
			return ashes::ImageLayout::eColourAttachmentOptimal;

		case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
			return ashes::ImageLayout::eDepthStencilAttachmentOptimal;

		case VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL:
			return ashes::ImageLayout::eDepthStencilReadOnlyOptimal;

		case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
			return ashes::ImageLayout::eShaderReadOnlyOptimal;

		case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
			return ashes::ImageLayout::eTransferSrcOptimal;

		case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
			return ashes::ImageLayout::eTransferDstOptimal;

		case VK_IMAGE_LAYOUT_PREINITIALIZED:
			return ashes::ImageLayout::ePreinitialised;

		case VK_IMAGE_LAYOUT_PRESENT_SRC_KHR:
			return ashes::ImageLayout::ePresentSrc;

		default:
			assert( false && "Unsupported image layout" );
			return ashes::ImageLayout::eUndefined;
		}
	}
}
