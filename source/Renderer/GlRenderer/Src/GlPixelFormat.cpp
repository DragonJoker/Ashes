#include "GlRendererPrerequisites.hpp"

namespace gl_renderer
{
	GLenum getInternal( renderer::PixelFormat const & format )noexcept
	{
		switch ( format )
		{
		case renderer::PixelFormat::eL8:
			return GL_R8;

		case renderer::PixelFormat::eL8A8:
			return GL_RG8;

		case renderer::PixelFormat::eR8G8B8:
			return GL_RGB8;

		case renderer::PixelFormat::eRGB565:
			return GL_RGB565;

		case renderer::PixelFormat::eR8G8B8A8:
			return GL_RGBA8;

		case renderer::PixelFormat::eB8G8R8A8:
			return GL_BGRA8_EXT;

		case renderer::PixelFormat::eRGBA5551:
			return GL_RGB5_A1;

		case renderer::PixelFormat::eRGBA4444:
			return GL_RGBA4;

		case renderer::PixelFormat::eD16:
			return GL_DEPTH_COMPONENT16;

		case renderer::PixelFormat::eD24S8:
			return GL_DEPTH24_STENCIL8;

		case renderer::PixelFormat::eD32F:
			return GL_DEPTH_COMPONENT32F;

		case renderer::PixelFormat::eS8:
			return GL_STENCIL_INDEX8;

		default:
			assert( false && "Unsupported pixel format." );
			return GL_RGBA8;
		}
	}

	GLenum getFormat( renderer::PixelFormat format )noexcept
	{
		switch ( format )
		{
		case renderer::PixelFormat::eL8:
			return GL_RED;

		case renderer::PixelFormat::eL8A8:
			return GL_RG;

		case renderer::PixelFormat::eR8G8B8:
			return GL_RGB;

		case renderer::PixelFormat::eRGB565:
			return GL_RGB;

		case renderer::PixelFormat::eR8G8B8A8:
			return GL_ABGR_EXT;

		case renderer::PixelFormat::eB8G8R8A8:
			return GL_BGRA;

		case renderer::PixelFormat::eRGBA5551:
			return GL_RGBA;

		case renderer::PixelFormat::eRGBA4444:
			return GL_RGBA;

		case renderer::PixelFormat::eD16:
		case renderer::PixelFormat::eD32F:
			return GL_DEPTH_COMPONENT;

		case renderer::PixelFormat::eD24S8:
			return GL_DEPTH_STENCIL;

		case renderer::PixelFormat::eS8:
			return GL_STENCIL;

		default:
			assert( false && "Unsupported pixel format." );
			return GL_RGBA8;
		}
	}

	GLenum getType( renderer::PixelFormat format )noexcept
	{
		switch ( format )
		{
		case renderer::PixelFormat::eL8:
			return GL_UNSIGNED_BYTE;

		case renderer::PixelFormat::eL8A8:
			return GL_UNSIGNED_BYTE;

		case renderer::PixelFormat::eR8G8B8:
			return GL_UNSIGNED_BYTE;

		case renderer::PixelFormat::eRGB565:
			return GL_UNSIGNED_SHORT_5_6_5;

		case renderer::PixelFormat::eR8G8B8A8:
			return GL_UNSIGNED_INT_8_8_8_8;

		case renderer::PixelFormat::eB8G8R8A8:
			return GL_UNSIGNED_INT_8_8_8_8;

		case renderer::PixelFormat::eRGBA5551:
			return GL_UNSIGNED_SHORT_5_5_5_1;

		case renderer::PixelFormat::eRGBA4444:
			return GL_UNSIGNED_SHORT_4_4_4_4;

		case renderer::PixelFormat::eD16:
			return GL_UNSIGNED_SHORT;

		case renderer::PixelFormat::eD24S8:
			return GL_UNSIGNED_INT_24_8;

		case renderer::PixelFormat::eD32F:
			return GL_FLOAT;

		case renderer::PixelFormat::eS8:
			return GL_UNSIGNED_BYTE;

		default:
			assert( false && "Unsupported pixel format." );
			return GL_RGBA8;
		}
	}

	uint32_t getSize( renderer::PixelFormat format )noexcept
	{
		switch ( format )
		{
		case renderer::PixelFormat::eL8:
			return 1u;

		case renderer::PixelFormat::eL8A8:
			return 2u;

		case renderer::PixelFormat::eR8G8B8:
			return 3u;

		case renderer::PixelFormat::eRGB565:
			return 2u;

		case renderer::PixelFormat::eR8G8B8A8:
			return 4u;

		case renderer::PixelFormat::eB8G8R8A8:
			return 4u;

		case renderer::PixelFormat::eRGBA5551:
			return 2u;

		case renderer::PixelFormat::eRGBA4444:
			return 2u;

		case renderer::PixelFormat::eD16:
			return 2u;

		case renderer::PixelFormat::eD24S8:
			return 4u;

		case renderer::PixelFormat::eD32F:
			return 4u;

		case renderer::PixelFormat::eS8:
			return 1u;

		default:
			assert( false && "Unsupported pixel format." );
			return 0u;
		}
	}

	renderer::PixelFormat convert( GLenum format )noexcept
	{
		switch ( format )
		{
		case GL_R8:
			return renderer::PixelFormat::eL8;

		case GL_RG8:
			return renderer::PixelFormat::eL8A8;

		case GL_RGB8:
			return renderer::PixelFormat::eR8G8B8;

		case GL_RGB565:
			return renderer::PixelFormat::eRGB565;

		case GL_RGBA8:
			return renderer::PixelFormat::eR8G8B8A8;

		case GL_BGRA:
			return renderer::PixelFormat::eB8G8R8A8;

		case GL_RGB5_A1:
			return renderer::PixelFormat::eRGBA5551;

		case GL_RGBA4:
			return renderer::PixelFormat::eRGBA4444;

		case GL_DEPTH_COMPONENT16:
			return renderer::PixelFormat::eD16;

		case GL_DEPTH24_STENCIL8:
			return renderer::PixelFormat::eD24S8;

		case GL_DEPTH_COMPONENT32F:
			return renderer::PixelFormat::eD32F;

		case GL_STENCIL_INDEX8:
			return renderer::PixelFormat::eS8;

		default:
			assert( false && "Unsupported pixel format." );
			return renderer::PixelFormat::eR8G8B8A8;
		}
	}
}
