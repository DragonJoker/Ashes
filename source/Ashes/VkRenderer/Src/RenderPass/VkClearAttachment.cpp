#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkClearAttachment convert( ashes::ClearAttachment const & value )
	{
		return VkClearAttachment
		{
			convert( value.aspectMask ),
			value.colourAttachment,
			convert( value.clearValue ),
		};
	}
}
