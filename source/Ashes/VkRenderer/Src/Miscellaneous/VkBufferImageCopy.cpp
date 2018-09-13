#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkBufferImageCopy convert( ashes::BufferImageCopy const & value )
	{
		return VkBufferImageCopy
		{
			value.bufferOffset,
			value.bufferRowLength,
			value.bufferImageHeight,
			convert( value.imageSubresource ),
			convert( value.imageOffset ),
			convert( value.imageExtent ),
		};
	}
}
