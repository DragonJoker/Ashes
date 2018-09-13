#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkImageAspectFlags convert( ashes::ImageAspectFlags const & flags )
	{
		VkImageAspectFlags result{ 0 };

		if ( checkFlag( flags, ashes::ImageAspectFlag::eColour ) )
		{
			result |= VK_IMAGE_ASPECT_COLOR_BIT;
		}

		if ( checkFlag( flags, ashes::ImageAspectFlag::eDepth ) )
		{
			result |= VK_IMAGE_ASPECT_DEPTH_BIT;
		}

		if ( checkFlag( flags, ashes::ImageAspectFlag::eStencil ) )
		{
			result |= VK_IMAGE_ASPECT_STENCIL_BIT;
		}

		if ( checkFlag( flags, ashes::ImageAspectFlag::eMetaData ) )
		{
			result |= VK_IMAGE_ASPECT_METADATA_BIT;
		}

		return result;
	}

	ashes::ImageAspectFlags convertAspectMask( VkImageAspectFlags const & flags )
	{
		ashes::ImageAspectFlags result{ 0 };

		if ( ashes::checkFlag( flags, VK_IMAGE_ASPECT_COLOR_BIT ) )
		{
			result |= ashes::ImageAspectFlag::eColour;
		}

		if ( ashes::checkFlag( flags, VK_IMAGE_ASPECT_DEPTH_BIT ) )
		{
			result |= ashes::ImageAspectFlag::eDepth;
		}

		if ( ashes::checkFlag( flags, VK_IMAGE_ASPECT_STENCIL_BIT ) )
		{
			result |= ashes::ImageAspectFlag::eStencil;
		}

		if ( ashes::checkFlag( flags, VK_IMAGE_ASPECT_METADATA_BIT ) )
		{
			result |= ashes::ImageAspectFlag::eMetaData;
		}

		return result;
	}
}
