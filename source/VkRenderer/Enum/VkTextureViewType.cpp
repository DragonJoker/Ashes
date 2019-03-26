#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkImageViewType convert( ashes::ImageViewType const & value )noexcept
	{
		switch ( value )
		{
		case ashes::ImageViewType::e1D:
			return VK_IMAGE_VIEW_TYPE_1D;

		case ashes::ImageViewType::e2D:
			return VK_IMAGE_VIEW_TYPE_2D;

		case ashes::ImageViewType::e3D:
			return VK_IMAGE_VIEW_TYPE_3D;

		case ashes::ImageViewType::eCube:
			return VK_IMAGE_VIEW_TYPE_CUBE;

		case ashes::ImageViewType::e1DArray:
			return VK_IMAGE_VIEW_TYPE_1D_ARRAY;

		case ashes::ImageViewType::e2DArray:
			return VK_IMAGE_VIEW_TYPE_2D_ARRAY;

		case ashes::ImageViewType::eCubeArray:
			return VK_IMAGE_VIEW_TYPE_CUBE_ARRAY;

		default:
			assert( false && "Unsupported ImageViewType" );
			return VK_IMAGE_VIEW_TYPE_2D;
		}
	}
}
