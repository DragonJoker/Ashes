#include "GlRendererPrerequisites.hpp"

namespace gl_renderer
{
	std::string getName( GlInternal value )
	{
		switch ( value )
		{
		case gl_renderer::GL_INTERNAL_R8G8B8:
			return "GL_RGB8";

		case gl_renderer::GL_INTERNAL_R8G8B8A8:
			return "GL_RGBA8";

		case gl_renderer::GL_INTERNAL_R4G4B4A4:
			return "GL_RGBA4";

		case gl_renderer::GL_INTERNAL_R5G5B5A1:
			return "GL_RGB5_A1";

		case gl_renderer::GL_INTERNAL_D16:
			return "GL_DEPTH_COMPONENT16";

		case gl_renderer::GL_INTERNAL_D24:
			return "GL_DEPTH_COMPONENT24";

		case gl_renderer::GL_INTERNAL_D32:
			return "GL_DEPTH_COMPONENT32";

		case gl_renderer::GL_INTERNAL_R8:
			return "GL_R8";

		case gl_renderer::GL_INTERNAL_R8G8:
			return "GL_RG8";

		case gl_renderer::GL_INTERNAL_R16F:
			return "GL_R16F";

		case gl_renderer::GL_INTERNAL_R32F:
			return "GL_R32F";

		case gl_renderer::GL_INTERNAL_RG16F:
			return "GL_RG16F";

		case gl_renderer::GL_INTERNAL_RG32F:
			return "GL_RG32F";

		case gl_renderer::GL_INTERNAL_DXT1:
			return "GL_COMPRESSED_S3TC_DXT1";

		case gl_renderer::GL_INTERNAL_DXT3:
			return "GL_COMPRESSED_S3TC_DXT3";

		case gl_renderer::GL_INTERNAL_DXT5:
			return "GL_COMPRESSED_S3TC_DXT5";

		case gl_renderer::GL_INTERNAL_D24S8:
			return "GL_DEPTH_COMPONENT24_S8";

		case gl_renderer::GL_INTERNAL_RGBA32F:
			return "GL_RGBA32F";

		case gl_renderer::GL_INTERNAL_RGB32F:
			return "GL_RGB32F";

		case gl_renderer::GL_INTERNAL_RGBA16F:
			return "GL_RGBA16F";

		case gl_renderer::GL_INTERNAL_RGB16F:
			return "GL_RGB16F";

		case gl_renderer::GL_INTERNAL_R8G8B8_SRGB:
			return "GL_SRGB8";

		case gl_renderer::GL_INTERNAL_R8G8B8A8_SRGB:
			return "GL_SRGBA8";

		case gl_renderer::GL_INTERNAL_D32F:
			return "GL_DEPTH_COMPONENT32F";

		case gl_renderer::GL_INTERNAL_D32FS8:
			return "GL_DEPTH32F_STENCIL8";

		case gl_renderer::GL_INTERNAL_S1:
			return "GL_STENCIL_INDEX1";

		case gl_renderer::GL_INTERNAL_S8:
			return "GL_STENCIL_INDEX8";

		case gl_renderer::GL_INTERNAL_R5G6B5:
			return "GL_RGB565";

		case gl_renderer::GL_INTERNAL_B8G8R8A8:
			return "GL_BGRA8";

		default:
			assert( false && "Unupported GlInternal" );
			return "GlInternal_UNKNOWN";
		}
	}

	std::string getName( GlFormat value )
	{
		switch ( value )
		{
		case GL_FORMAT_S:
			return "GL_STENCIL_INDEX";

		case GL_FORMAT_D:
			return "GL_DEPTH_COMPONENT";

		case GL_FORMAT_R:
			return "GL_RED";

		case GL_FORMAT_RGB:
			return "GL_RGB";

		case GL_FORMAT_RGBA:
			return "GL_RGBA";

		case GL_FORMAT_ABGR:
			return "GL_ABGR";

		case GL_FORMAT_BGR:
			return "GL_BGR";

		case GL_FORMAT_BGRA:
			return "GL_BGRA";

		case GL_FORMAT_RG:
			return "GL_RG";

		case GL_FORMAT_DS:
			return "GL_DEPTH_STENCIL";

		default:
			assert( false && "Unupported GlFormat" );
			return "GlFormat_UNKNOWN";
		}
	}

	std::string getName( GlType value )
	{
		switch ( value )
		{
		case GL_TYPE_UI8:
			return "GL_UNSIGNED_BYTE";

		case GL_TYPE_UI16:
			return "GL_UNSIGNED_SHORT";

		case GL_TYPE_F:
			return "GL_FLOAT";

		case GL_TYPE_US4444:
			return "GL_UNSIGNED_SHORT_4_4_4_4";

		case GL_TYPE_US5551:
			return "GL_UNSIGNED_SHORT_5_5_5_1";

		case GL_TYPE_UI8888:
			return "GL_UNSIGNED_INT_8_8_8_8";

		case GL_TYPE_UI565:
			return "GL_UNSIGNED_SHORT_5_6_5";

		case GL_TYPE_UI24_8:
			return "GL_UNSIGNED_INT_24_8";

		default:
			assert( false && "Unupported GlType" );
			return "GlType_UNKNOWN";
		}
	}

	GlInternal getInternal( renderer::PixelFormat const & format )noexcept
	{
		switch ( format )
		{
		case renderer::PixelFormat::eR8:
			return GL_INTERNAL_R8;

		case renderer::PixelFormat::eR32F:
			return GL_INTERNAL_R32F;

		case renderer::PixelFormat::eRG32F:
			return GL_INTERNAL_RG32F;

		case renderer::PixelFormat::eRGB32F:
			return GL_INTERNAL_RGB32F;

		case renderer::PixelFormat::eR8G8:
			return GL_INTERNAL_R8G8;

		case renderer::PixelFormat::eR8G8B8:
			return GL_INTERNAL_R8G8B8;

		case renderer::PixelFormat::eR8G8B8A8:
			return GL_INTERNAL_R8G8B8A8;

		case renderer::PixelFormat::eB8G8R8A8:
			return GL_INTERNAL_B8G8R8A8;

		case renderer::PixelFormat::eRGBA32F:
			return GL_INTERNAL_RGBA32F;

		case renderer::PixelFormat::eD16:
			return GL_INTERNAL_D16;

		case renderer::PixelFormat::eD24S8:
			return GL_INTERNAL_D24S8;

		case renderer::PixelFormat::eD32F:
			return GL_INTERNAL_D32F;

		case renderer::PixelFormat::eD32FS8:
			return GL_INTERNAL_D32FS8;

		case renderer::PixelFormat::eS8:
			return GL_INTERNAL_S8;

		default:
			assert( false && "Unsupported pixel format." );
			return GL_INTERNAL_R8G8B8A8;
		}
	}

	GlFormat getFormat( renderer::PixelFormat format )noexcept
	{
		switch ( format )
		{
		case renderer::PixelFormat::eR8:
			return GL_FORMAT_R;

		case renderer::PixelFormat::eR8G8:
			return GL_FORMAT_RG;

		case renderer::PixelFormat::eR32F:
			return GL_FORMAT_R;

		case renderer::PixelFormat::eR8G8B8:
			return GL_FORMAT_RGB;

		case renderer::PixelFormat::eR8G8B8A8:
#if defined( _WIN32 ) || defined( __linux__ )
			return GL_FORMAT_ABGR;
#else
			return GL_FORMAT_RGBA;
#endif

		case renderer::PixelFormat::eB8G8R8A8:
			return GL_FORMAT_BGRA;

		case renderer::PixelFormat::eRGBA32F:
			return GL_FORMAT_RGBA;

		case renderer::PixelFormat::eD16:
		case renderer::PixelFormat::eD32F:
			return GL_FORMAT_D;

		case renderer::PixelFormat::eD24S8:
		case renderer::PixelFormat::eD32FS8:
			return GL_FORMAT_DS;

		case renderer::PixelFormat::eS8:
			return GL_FORMAT_S;

		default:
			assert( false && "Unsupported pixel format." );
			return GL_FORMAT_RGBA;
		}
	}

	GlType getType( renderer::PixelFormat format )noexcept
	{
		switch ( format )
		{
		case renderer::PixelFormat::eR8:
			return GL_TYPE_UI8;

		case renderer::PixelFormat::eR8G8:
			return GL_TYPE_UI8;

		case renderer::PixelFormat::eR32F:
			return GL_TYPE_F;

		case renderer::PixelFormat::eR8G8B8:
			return GL_TYPE_UI8;

		case renderer::PixelFormat::eR8G8B8A8:
			return GL_TYPE_UI8888;

		case renderer::PixelFormat::eB8G8R8A8:
			return GL_TYPE_UI8888;

		case renderer::PixelFormat::eRGBA32F:
			return GL_TYPE_F;

		case renderer::PixelFormat::eD16:
			return GL_TYPE_UI16;

		case renderer::PixelFormat::eD24S8:
			return GL_TYPE_UI24_8;

		case renderer::PixelFormat::eD32F:
			return GL_TYPE_F;

		case renderer::PixelFormat::eD32FS8:
			return GL_TYPE_32F_UI24_8;

		case renderer::PixelFormat::eS8:
			return GL_TYPE_UI8;

		default:
			assert( false && "Unsupported pixel format." );
			return GL_TYPE_UI8888;
		}
	}

	renderer::PixelFormat convert( GlInternal format )noexcept
	{
		switch ( format )
		{
		case GL_INTERNAL_R8:
			return renderer::PixelFormat::eR8;

		case GL_INTERNAL_R8G8:
			return renderer::PixelFormat::eR8G8;

		case GL_INTERNAL_R32F:
			return renderer::PixelFormat::eR32F;

		case GL_INTERNAL_R8G8B8:
			return renderer::PixelFormat::eR8G8B8;

		case GL_INTERNAL_R8G8B8A8:
			return renderer::PixelFormat::eR8G8B8A8;

		case GL_INTERNAL_B8G8R8A8:
			return renderer::PixelFormat::eB8G8R8A8;

		case GL_INTERNAL_RGBA32F:
			return renderer::PixelFormat::eRGBA32F;

		case GL_INTERNAL_D16:
			return renderer::PixelFormat::eD16;

		case GL_INTERNAL_D24S8:
			return renderer::PixelFormat::eD24S8;

		case GL_INTERNAL_D32F:
			return renderer::PixelFormat::eD32F;

		case GL_INTERNAL_D32FS8:
			return renderer::PixelFormat::eD32FS8;

		case GL_INTERNAL_S8:
			return renderer::PixelFormat::eS8;

		default:
			assert( false && "Unsupported pixel format." );
			return renderer::PixelFormat::eR8G8B8A8;
		}
	}
}
