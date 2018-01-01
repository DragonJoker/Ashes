#include "GlPixelFormat.hpp"

namespace gl_renderer
{
	GLenum convert( utils::PixelFormat const & format )noexcept
	{
		switch ( format )
		{
		case utils::PixelFormat::eL8:
			return GL_R8;

		case utils::PixelFormat::eL8A8:
			return GL_RG8;

		case utils::PixelFormat::eR8G8B8:
			return GL_RGB8;

		case utils::PixelFormat::eRGB565:
			return GL_RGB565;

		case utils::PixelFormat::eR8G8B8A8:
			return GL_RGBA8;

		case utils::PixelFormat::eB8G8R8A8:
			return GL_BGRA8_EXT;

		case utils::PixelFormat::eRGBA5551:
			return GL_RGB5_A1;

		case utils::PixelFormat::eRGBA4444:
			return GL_RGBA4;

		case utils::PixelFormat::eD16:
			return GL_DEPTH_COMPONENT16;

		case utils::PixelFormat::eD24S8:
			return GL_DEPTH24_STENCIL8;

		case utils::PixelFormat::eS8:
			return GL_STENCIL_INDEX8;

		default:
			assert( false && "Unsupported pixel format." );
			return GL_RGBA8;
		}
	}

	utils::PixelFormat convert( GLenum format )noexcept
	{
		switch ( format )
		{
		case GL_R8:
			return utils::PixelFormat::eL8;

		case GL_RG8:
			return utils::PixelFormat::eL8A8;

		case GL_RGB8:
			return utils::PixelFormat::eR8G8B8;

		case GL_RGB565:
			return utils::PixelFormat::eRGB565;

		case GL_RGBA8:
			return utils::PixelFormat::eR8G8B8A8;

		case GL_BGRA:
			return utils::PixelFormat::eB8G8R8A8;

		case GL_RGB5_A1:
			return utils::PixelFormat::eRGBA5551;

		case GL_RGBA4:
			return utils::PixelFormat::eRGBA4444;

		case GL_DEPTH_COMPONENT16:
			return utils::PixelFormat::eD16;

		case GL_DEPTH24_STENCIL8:
			return utils::PixelFormat::eD24S8;

		case GL_STENCIL_INDEX8:
			return utils::PixelFormat::eS8;

		default:
			assert( false && "Unsupported pixel format." );
			return utils::PixelFormat::eR8G8B8A8;
		}
	}
}
