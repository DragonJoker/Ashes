#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkSharingMode convert( ashes::SharingMode const & value )
	{
		switch ( value )
		{
		case ashes::SharingMode::eExclusive:
			return VK_SHARING_MODE_EXCLUSIVE;

		case ashes::SharingMode::eConcurrent:
			return VK_SHARING_MODE_CONCURRENT;

		default:
			assert( false && "Unsupported SharingMode" );
			return VK_SHARING_MODE_EXCLUSIVE;
		}
	}
}
