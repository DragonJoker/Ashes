#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkImageSubresourceRange convert( ashes::ImageSubresourceRange const & range )
	{
		return VkImageSubresourceRange
		{
			convert( range.aspectMask ),
			range.baseMipLevel,
			range.levelCount,
			range.baseArrayLayer,
			range.layerCount
		};
	}

	ashes::ImageSubresourceRange convert( VkImageSubresourceRange const & range )
	{
		return ashes::ImageSubresourceRange
		{
			convertAspectMask( range.aspectMask ),
			range.baseMipLevel,
			range.levelCount,
			range.baseArrayLayer,
			range.layerCount
		};
	}
}
