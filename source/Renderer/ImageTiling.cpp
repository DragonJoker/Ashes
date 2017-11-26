#include "ImageTiling.hpp"

namespace renderer
{
	VkImageTiling convert( ImageTiling const & value )
	{
		switch ( value )
		{
		case ImageTiling::eOptimal:
			return VK_IMAGE_TILING_OPTIMAL;

		case ImageTiling::eLinear:
			return VK_IMAGE_TILING_LINEAR;

		default:
			assert( false && "Unsupported image tiling" );
			return VK_IMAGE_TILING_OPTIMAL;
		}
	}
}
