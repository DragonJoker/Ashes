#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkImageUsageFlags convert( renderer::ImageUsageFlags const & flags )
	{
		VkImageUsageFlags result{ 0 };

		if ( checkFlag( flags, renderer::ImageUsageFlag::eTransferSrc ) )
		{
			result |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
		}

		if ( checkFlag( flags, renderer::ImageUsageFlag::eTransferDst ) )
		{
			result |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
		}

		if ( checkFlag( flags, renderer::ImageUsageFlag::eSampled ) )
		{
			result |= VK_IMAGE_USAGE_SAMPLED_BIT;
		}

		if ( checkFlag( flags, renderer::ImageUsageFlag::eStorage ) )
		{
			result |= VK_IMAGE_USAGE_STORAGE_BIT;
		}

		if ( checkFlag( flags, renderer::ImageUsageFlag::eColourAttachment ) )
		{
			result |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		}

		if ( checkFlag( flags, renderer::ImageUsageFlag::eDepthStencilAttachment ) )
		{
			result |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
		}

		if ( checkFlag( flags, renderer::ImageUsageFlag::eTransientAttachment ) )
		{
			result |= VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT;
		}

		if ( checkFlag( flags, renderer::ImageUsageFlag::eInputAttachment ) )
		{
			result |= VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT;
		}

		return result;
	}

	renderer::ImageUsageFlags convertImageUsageFlags( VkImageUsageFlags const & flags )
	{
		renderer::ImageUsageFlags result{ 0 };

		if ( renderer::checkFlag( flags, VK_IMAGE_USAGE_TRANSFER_SRC_BIT ) )
		{
			result |= renderer::ImageUsageFlag::eTransferSrc;
		}

		if ( renderer::checkFlag( flags, VK_IMAGE_USAGE_TRANSFER_DST_BIT ) )
		{
			result |= renderer::ImageUsageFlag::eTransferDst;
		}

		if ( renderer::checkFlag( flags, VK_IMAGE_USAGE_SAMPLED_BIT ) )
		{
			result |= renderer::ImageUsageFlag::eSampled;
		}

		if ( renderer::checkFlag( flags, VK_IMAGE_USAGE_STORAGE_BIT ) )
		{
			result |= renderer::ImageUsageFlag::eStorage;
		}

		if ( renderer::checkFlag( flags, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT ) )
		{
			result |= renderer::ImageUsageFlag::eColourAttachment;
		}

		if ( renderer::checkFlag( flags, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT ) )
		{
			result |= renderer::ImageUsageFlag::eDepthStencilAttachment;
		}

		if ( renderer::checkFlag( flags, VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT ) )
		{
			result |= renderer::ImageUsageFlag::eTransientAttachment;
		}

		if ( renderer::checkFlag( flags, VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT ) )
		{
			result |= renderer::ImageUsageFlag::eInputAttachment;
		}

		return result;
	}
}
