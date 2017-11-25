#include "ImageLayout.hpp"

namespace renderer
{
	VkImageLayout convert( ImageLayout const & layout )
	{
		switch ( layout )
		{
		case ImageLayout::eUndefined:
			return VK_IMAGE_LAYOUT_UNDEFINED;

		case ImageLayout::eGeneral:
			return VK_IMAGE_LAYOUT_GENERAL;

		case ImageLayout::eColourAttachmentOptimal:
			return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		case ImageLayout::eDepthStencilAttachmentOptimal:
			return VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		case ImageLayout::eDepthStencilReadOnlyOptimal:
			return VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;

		case ImageLayout::eShaderReadOnlyOptimal:
			return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

		case ImageLayout::eTransferSrcOptimal:
			return VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;

		case ImageLayout::eTransferDstOptimal:
			return VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;

		case ImageLayout::ePreinitialised:
			return VK_IMAGE_LAYOUT_PREINITIALIZED;

		case ImageLayout::ePresentSrc:
			return VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		default:
			assert( false && "Unsupported image layout" );
			return VK_IMAGE_LAYOUT_UNDEFINED;
		}
	}
}
