#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkImageTiling convert( ashes::ImageTiling const & value )
	{
		switch ( value )
		{
		case ashes::ImageTiling::eOptimal:
			return VK_IMAGE_TILING_OPTIMAL;

		case ashes::ImageTiling::eLinear:
			return VK_IMAGE_TILING_LINEAR;

		default:
			assert( false && "Unsupported image tiling" );
			return VK_IMAGE_TILING_OPTIMAL;
		}
	}
}
