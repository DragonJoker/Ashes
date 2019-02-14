#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkImageSubresourceLayers convert( ashes::ImageSubresourceLayers const & value )
	{
		return VkImageSubresourceLayers
		{
			convert( value.aspectMask ),
			value.mipLevel,
			value.baseArrayLayer,
			value.layerCount
		};
	}
}
