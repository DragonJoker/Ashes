#include "ImageAspectFlag.hpp"

namespace renderer
{
	VkImageAspectFlags convert( ImageAspectFlags const & flags )
	{
		VkImageAspectFlags result{ 0 };

		if ( checkFlag( flags, ImageAspectFlag::eColour ) )
		{
			result |= VK_IMAGE_ASPECT_COLOR_BIT;
		}

		if ( checkFlag( flags, ImageAspectFlag::eDepth ) )
		{
			result |= VK_IMAGE_ASPECT_DEPTH_BIT;
		}

		if ( checkFlag( flags, ImageAspectFlag::eStencil ) )
		{
			result |= VK_IMAGE_ASPECT_STENCIL_BIT;
		}

		if ( checkFlag( flags, ImageAspectFlag::eMetaData ) )
		{
			result |= VK_IMAGE_ASPECT_METADATA_BIT;
		}

		return result;
	}
}
