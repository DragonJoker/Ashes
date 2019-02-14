#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkImageSubresource convert( ashes::ImageSubresource const & range )
	{
		return VkImageSubresource
		{
			convert( range.aspectMask ),
			range.mipLevel,
			range.arrayLayer
		};
	}

	ashes::ImageSubresource convert( VkImageSubresource const & range )
	{
		return ashes::ImageSubresource
		{
			convertAspectMask( range.aspectMask ),
			range.mipLevel,
			range.arrayLayer
		};
	}
}
