#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkSubpassContents convert( ashes::SubpassContents const & value )
	{
		switch ( value )
		{
		case ashes::SubpassContents::eInline:
			return VK_SUBPASS_CONTENTS_INLINE;

		case ashes::SubpassContents::eSecondaryCommandBuffers:
			return VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS;

		default:
			assert( false && "Unsupported SubpassContents" );
			return VK_SUBPASS_CONTENTS_INLINE;
		}
	}
}
