#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkImageViewType convert( ashes::TextureViewType const & value )noexcept
	{
		switch ( value )
		{
		case ashes::TextureViewType::e1D:
			return VK_IMAGE_VIEW_TYPE_1D;

		case ashes::TextureViewType::e2D:
			return VK_IMAGE_VIEW_TYPE_2D;

		case ashes::TextureViewType::e3D:
			return VK_IMAGE_VIEW_TYPE_3D;

		case ashes::TextureViewType::eCube:
			return VK_IMAGE_VIEW_TYPE_CUBE;

		case ashes::TextureViewType::e1DArray:
			return VK_IMAGE_VIEW_TYPE_1D_ARRAY;

		case ashes::TextureViewType::e2DArray:
			return VK_IMAGE_VIEW_TYPE_2D_ARRAY;

		case ashes::TextureViewType::eCubeArray:
			return VK_IMAGE_VIEW_TYPE_CUBE_ARRAY;

		default:
			assert( false && "Unsupported TextureViewType" );
			return VK_IMAGE_VIEW_TYPE_2D;
		}
	}
}
