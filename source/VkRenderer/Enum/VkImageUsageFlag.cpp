#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkImageUsageFlags convert( ashes::ImageUsageFlags const & flags )
	{
		VkImageUsageFlags result{ 0 };

		if ( checkFlag( flags, ashes::ImageUsageFlag::eTransferSrc ) )
		{
			result |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
		}

		if ( checkFlag( flags, ashes::ImageUsageFlag::eTransferDst ) )
		{
			result |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
		}

		if ( checkFlag( flags, ashes::ImageUsageFlag::eSampled ) )
		{
			result |= VK_IMAGE_USAGE_SAMPLED_BIT;
		}

		if ( checkFlag( flags, ashes::ImageUsageFlag::eStorage ) )
		{
			result |= VK_IMAGE_USAGE_STORAGE_BIT;
		}

		if ( checkFlag( flags, ashes::ImageUsageFlag::eColourAttachment ) )
		{
			result |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		}

		if ( checkFlag( flags, ashes::ImageUsageFlag::eDepthStencilAttachment ) )
		{
			result |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
		}

		if ( checkFlag( flags, ashes::ImageUsageFlag::eTransientAttachment ) )
		{
			result |= VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT;
		}

		if ( checkFlag( flags, ashes::ImageUsageFlag::eInputAttachment ) )
		{
			result |= VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT;
		}

		return result;
	}

	ashes::ImageUsageFlags convertImageUsageFlags( VkImageUsageFlags const & flags )
	{
		ashes::ImageUsageFlags result{ 0 };

		if ( ashes::checkFlag( flags, VK_IMAGE_USAGE_TRANSFER_SRC_BIT ) )
		{
			result |= ashes::ImageUsageFlag::eTransferSrc;
		}

		if ( ashes::checkFlag( flags, VK_IMAGE_USAGE_TRANSFER_DST_BIT ) )
		{
			result |= ashes::ImageUsageFlag::eTransferDst;
		}

		if ( ashes::checkFlag( flags, VK_IMAGE_USAGE_SAMPLED_BIT ) )
		{
			result |= ashes::ImageUsageFlag::eSampled;
		}

		if ( ashes::checkFlag( flags, VK_IMAGE_USAGE_STORAGE_BIT ) )
		{
			result |= ashes::ImageUsageFlag::eStorage;
		}

		if ( ashes::checkFlag( flags, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT ) )
		{
			result |= ashes::ImageUsageFlag::eColourAttachment;
		}

		if ( ashes::checkFlag( flags, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT ) )
		{
			result |= ashes::ImageUsageFlag::eDepthStencilAttachment;
		}

		if ( ashes::checkFlag( flags, VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT ) )
		{
			result |= ashes::ImageUsageFlag::eTransientAttachment;
		}

		if ( ashes::checkFlag( flags, VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT ) )
		{
			result |= ashes::ImageUsageFlag::eInputAttachment;
		}

		return result;
	}
}
