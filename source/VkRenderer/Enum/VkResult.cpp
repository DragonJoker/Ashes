#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkResult convert( ashes::Result const & value )
	{
		return VkResult( value );
	}

	ashes::Result convert( VkResult const & value )
	{
		return ashes::Result( value );
	}
}
