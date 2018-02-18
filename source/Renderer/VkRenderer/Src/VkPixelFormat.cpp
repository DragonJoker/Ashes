#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkFormat convert( renderer::PixelFormat const & format )noexcept
	{
		switch ( format )
		{
		case renderer::PixelFormat::eR8:
			return VK_FORMAT_R8_UINT;

		case renderer::PixelFormat::eR32F:
			return VK_FORMAT_R32_SFLOAT;

		case renderer::PixelFormat::eR8G8:
			return VK_FORMAT_R8G8_UINT;

		case renderer::PixelFormat::eRG32F:
			return VK_FORMAT_R32G32_SFLOAT;

		case renderer::PixelFormat::eR8G8B8:
			return VK_FORMAT_R8G8B8_UINT;

		case renderer::PixelFormat::eRGB32F:
			return VK_FORMAT_R32G32B32_SFLOAT;

		case renderer::PixelFormat::eR8G8B8A8:
			return VK_FORMAT_R8G8B8A8_UNORM;

		case renderer::PixelFormat::eB8G8R8A8:
			return VK_FORMAT_B8G8R8A8_UNORM;

		case renderer::PixelFormat::eRGBA32F:
			return VK_FORMAT_R32G32B32A32_SFLOAT;

		case renderer::PixelFormat::eD16:
			return VK_FORMAT_D16_UNORM;

		case renderer::PixelFormat::eD24S8:
			return VK_FORMAT_D24_UNORM_S8_UINT;

		case renderer::PixelFormat::eD32F:
			return VK_FORMAT_D32_SFLOAT;

		case renderer::PixelFormat::eS8:
			return VK_FORMAT_S8_UINT;

		default:
			assert( false && "Unsupported pixel format." );
			return VK_FORMAT_UNDEFINED;
		}
	}

	renderer::PixelFormat convert( VkFormat const & format )noexcept
	{
		switch ( format )
		{
		case VK_FORMAT_R8_UINT:
			return renderer::PixelFormat::eR8;

		case VK_FORMAT_R32_SFLOAT:
			return renderer::PixelFormat::eR32F;

		case VK_FORMAT_R8G8_UINT:
			return renderer::PixelFormat::eR8G8;

		case VK_FORMAT_R8G8B8_UINT:
			return renderer::PixelFormat::eR8G8B8;

		case VK_FORMAT_R8G8B8A8_UNORM:
			return renderer::PixelFormat::eR8G8B8A8;

		case VK_FORMAT_B8G8R8A8_UNORM:
			return renderer::PixelFormat::eB8G8R8A8;

		case VK_FORMAT_R32G32B32A32_SFLOAT:
			return renderer::PixelFormat::eRGBA32F;

		case VK_FORMAT_D16_UNORM:
			return renderer::PixelFormat::eD16;

		case VK_FORMAT_D24_UNORM_S8_UINT:
			return renderer::PixelFormat::eD24S8;

		case VK_FORMAT_D32_SFLOAT:
			return renderer::PixelFormat::eD32F;

		case VK_FORMAT_S8_UINT:
			return renderer::PixelFormat::eS8;

		default:
			assert( false && "Unsupported pixel format." );
			return renderer::PixelFormat::eR8G8B8A8;
		}
	}
}
