#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkFilter convert( ashes::Filter const & filter )
	{
		switch ( filter )
		{
		case ashes::Filter::eNearest:
			return VK_FILTER_NEAREST;

		case ashes::Filter::eLinear:
			return VK_FILTER_LINEAR;

		default:
			assert( false );
			return VK_FILTER_NEAREST;
		}
	}

}
