#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkImageType convert( ashes::TextureType const & value )noexcept
	{
		switch ( value )
		{
		case ashes::TextureType::e1D:
			return VK_IMAGE_TYPE_1D;

		case ashes::TextureType::e2D:
			return VK_IMAGE_TYPE_2D;

		case ashes::TextureType::e3D:
			return VK_IMAGE_TYPE_3D;

		default:
			assert( false && "Unsupported texture type" );
			return VK_IMAGE_TYPE_2D;
		}
	}
}
