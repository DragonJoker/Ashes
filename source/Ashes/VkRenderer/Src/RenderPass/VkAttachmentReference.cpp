#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkAttachmentReference convert( ashes::AttachmentReference const & value )
	{
		return VkAttachmentReference
		{
			value.attachment,
			convert( value.layout )
		};
	}
}
