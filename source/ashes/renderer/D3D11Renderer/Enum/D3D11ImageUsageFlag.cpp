#include "D3D11RendererPrerequisites.hpp"

namespace ashes::D3D11_NAMESPACE
{
	bool isRenderTarget( VkImageUsageFlags const & flags )noexcept
	{
		return checkFlag( flags, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT )
			|| checkFlag( flags, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT )
			|| checkFlag( flags, VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT )
			|| checkFlag( flags, VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT );
	}

	bool isStorage( VkImageUsageFlags const & flags )noexcept
	{
		return checkFlag( flags, VK_IMAGE_USAGE_STORAGE_BIT );
	}

	bool isSampled( VkImageUsageFlags const & flags )noexcept
	{
		return checkFlag( flags, VK_IMAGE_USAGE_SAMPLED_BIT )
			|| checkFlag( flags, VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT )
			/*|| checkFlag( flags, VK_IMAGE_USAGE_TRANSFER_SRC_BIT )*/;
	}

	bool isRenderable( VkImageUsageFlags const & flags
		, VkFormat format
		, uint32_t mipLevels )noexcept
	{
		return isRenderTarget( flags )
			|| ( mipLevels > 1 && !isCompressedFormat( format ) );
	}

	bool isMipmapped( VkImageUsageFlags const & flags
		, VkFormat format
		, uint32_t mipLevels )noexcept
	{
		return isRenderable( flags, format, mipLevels )
			&& isSampled( flags )
			&& mipLevels > 1
			&& !isDepthOrStencilFormat( format );
	}
}
