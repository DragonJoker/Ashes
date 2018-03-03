#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkImageBlit convert( renderer::ImageBlit const & value )
	{
		return VkImageBlit
		{
			convert( value.srcSubresource ),
			{
				{ value.srcOffset[0], value.srcOffset[1], value.srcOffset[2] },
				{ value.srcExtent[0], value.srcExtent[1], value.srcExtent[2] },
			},
			convert( value.dstSubresource ),
			{
				{ value.dstOffset[0], value.dstOffset[1], value.dstOffset[2] },
				{ value.dstExtent[0], value.dstExtent[1], value.dstExtent[2] },
			},
		};
	}
}
