#include "SubpassContents.hpp"

namespace renderer
{
	VkSubpassContents convert( SubpassContents const & value )
	{
		switch ( value )
		{
		case SubpassContents::eInline:
			return VK_SUBPASS_CONTENTS_INLINE;

		case SubpassContents::eSecondaryCommandBuffers:
			return VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS;

		default:
			assert( false && "Unsupported SubpassContents" );
			return VK_SUBPASS_CONTENTS_INLINE;
		}
	}
}
