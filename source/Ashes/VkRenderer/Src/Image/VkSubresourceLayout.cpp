#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkSubresourceLayout convert( ashes::SubresourceLayout const & range )
	{
		return VkSubresourceLayout
		{
			range.offset,
			range.size,
			range.rowPitch,
			range.arrayPitch,
			range.depthPitch
		};
	}

	ashes::SubresourceLayout convert( VkSubresourceLayout const & range )
	{
		return ashes::SubresourceLayout
		{
			range.offset,
			range.size,
			range.rowPitch,
			range.arrayPitch,
			range.depthPitch
		};
	}
}
