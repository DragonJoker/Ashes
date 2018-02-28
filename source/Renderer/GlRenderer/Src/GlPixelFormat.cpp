#include "GlRendererPrerequisites.hpp"

namespace gl_renderer
{
	std::string getName( GlInternal value )
	{
		switch ( value )
		{
		case GL_INTERNAL_R8G8B8:
			return "GL_RGB8";

		case GL_INTERNAL_R8G8B8A8:
			return "GL_RGBA8";

		case GL_INTERNAL_R4G4B4A4:
			return "GL_RGBA4";

		case GL_INTERNAL_R5G5B5A1:
			return "GL_RGB5_A1";

		case GL_INTERNAL_D16:
			return "GL_DEPTH_COMPONENT16";

		case GL_INTERNAL_D24:
			return "GL_DEPTH_COMPONENT24";

		case GL_INTERNAL_D32:
			return "GL_DEPTH_COMPONENT32";

		case GL_INTERNAL_R8:
			return "GL_R8";

		case GL_INTERNAL_R8G8:
			return "GL_RG8";

		case GL_INTERNAL_R16F:
			return "GL_R16F";

		case GL_INTERNAL_R32F:
			return "GL_R32F";

		case GL_INTERNAL_RG16F:
			return "GL_RG16F";

		case GL_INTERNAL_RG32F:
			return "GL_RG32F";

		case GL_INTERNAL_D24S8:
			return "GL_DEPTH_COMPONENT24_S8";

		case GL_INTERNAL_RGBA32F:
			return "GL_RGBA32F";

		case GL_INTERNAL_RGB32F:
			return "GL_RGB32F";

		case GL_INTERNAL_RGBA16F:
			return "GL_RGBA16F";

		case GL_INTERNAL_RGB16F:
			return "GL_RGB16F";

		case GL_INTERNAL_R8G8B8_SRGB:
			return "GL_SRGB8";

		case GL_INTERNAL_R8G8B8A8_SRGB:
			return "GL_SRGBA8";

		case GL_INTERNAL_D32F:
			return "GL_DEPTH_COMPONENT32F";

		case GL_INTERNAL_D32FS8:
			return "GL_DEPTH32F_STENCIL8";

		case GL_INTERNAL_S1:
			return "GL_STENCIL_INDEX1";

		case GL_INTERNAL_S8:
			return "GL_STENCIL_INDEX8";

		case GL_INTERNAL_R5G6B5:
			return "GL_COMPRESSED_R5G6B5";

		case GL_INTERNAL_B8G8R8A8:
			return "GL_COMPRESSED_B8G8R8A8";

		case GL_INTERNAL_RGB_S3TC_DXT1:
			return "GL_COMPRESSED_RGB_S3TC_DXT1";

		case GL_INTERNAL_RGBA_S3TC_DXT1:
			return "GL_COMPRESSED_RGBA_S3TC_DXT1";

		case GL_INTERNAL_RGBA_S3TC_DXT3:
			return "GL_COMPRESSED_RGBA_S3TC_DXT3";

		case GL_INTERNAL_RGBA_S3TC_DXT5:
			return "GL_COMPRESSED_RGBA_S3TC_DXT5";

		case GL_INTERNAL_SRGB_S3TC_DXT1:
			return "GL_COMPRESSED_SRGB_S3TC_DXT1";

		case GL_INTERNAL_SRGB_ALPHA_S3TC_DXT1:
			return "GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1";

		case GL_INTERNAL_SRGB_ALPHA_S3TC_DXT3:
			return "GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3";

		case GL_INTERNAL_SRGB_ALPHA_S3TC_DXT5:
			return "GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5";

		case GL_INTERNAL_RED_RGTC1:
			return "GL_COMPRESSED_RED_RGTC1";

		case GL_INTERNAL_SIGNED_RED_RGTC1:
			return "GL_COMPRESSED_SIGNED_RED_RGTC1";

		case GL_INTERNAL_RG_RGTC2:
			return "GL_COMPRESSED_RG_RGTC2";

		case GL_INTERNAL_SIGNED_RG_RGTC2:
			return "GL_COMPRESSED_SIGNED_RG_RGTC2";

		case GL_INTERNAL_RGB8_ETC2:
			return "GL_COMPRESSED_RGB8_ETC2";

		case GL_INTERNAL_SRGB8_ETC2:
			return "GL_COMPRESSED_SRGB8_ETC2";

		case GL_INTERNAL_RGB8_PUNCHTHROUGH_ALPHA1_ETC2:
			return "GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2";

		case GL_INTERNAL_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2:
			return "GL_COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2";

		case GL_INTERNAL_RGBA8_ETC2_EAC:
			return "GL_COMPRESSED_RGBA8_ETC2_EAC";

		case GL_INTERNAL_SRGB8_ALPHA8_ETC2_EAC:
			return "GL_COMPRESSED_SRGB8_ALPHA8_ETC2_EAC";

		case GL_INTERNAL_R11_EAC:
			return "GL_COMPRESSED_R11_EAC";

		case GL_INTERNAL_SIGNED_R11_EAC:
			return "GL_COMPRESSED_SIGNED_R11_EAC";

		case GL_INTERNAL_RG11_EAC:
			return "GL_COMPRESSED_RG11_EAC";

		case GL_INTERNAL_SIGNED_RG11_EAC:
			return "GL_COMPRESSED_SIGNED_RG11_EAC";

		case GL_INTERNAL_RGBA_ASTC_4x4:
			return "GL_COMPRESSED_RGBA_ASTC_4x4";

		case GL_INTERNAL_RGBA_ASTC_5x4:
			return "GL_COMPRESSED_RGBA_ASTC_5x4";

		case GL_INTERNAL_RGBA_ASTC_5x5:
			return "GL_COMPRESSED_RGBA_ASTC_5x5";

		case GL_INTERNAL_RGBA_ASTC_6x5:
			return "GL_COMPRESSED_RGBA_ASTC_6x5";

		case GL_INTERNAL_RGBA_ASTC_6x6:
			return "GL_COMPRESSED_RGBA_ASTC_6x6";

		case GL_INTERNAL_RGBA_ASTC_8x5:
			return "GL_COMPRESSED_RGBA_ASTC_8x5";

		case GL_INTERNAL_RGBA_ASTC_8x6:
			return "GL_COMPRESSED_RGBA_ASTC_8x6";

		case GL_INTERNAL_RGBA_ASTC_8x8:
			return "GL_COMPRESSED_RGBA_ASTC_8x8";

		case GL_INTERNAL_RGBA_ASTC_10x5:
			return "GL_COMPRESSED_RGBA_ASTC_10x5";

		case GL_INTERNAL_RGBA_ASTC_10x6:
			return "GL_COMPRESSED_RGBA_ASTC_10x6";

		case GL_INTERNAL_RGBA_ASTC_10x8:
			return "GL_COMPRESSED_RGBA_ASTC_10x8";

		case GL_INTERNAL_RGBA_ASTC_10x10:
			return "GL_COMPRESSED_RGBA_ASTC_10x10";

		case GL_INTERNAL_RGBA_ASTC_12x10:
			return "GL_COMPRESSED_RGBA_ASTC_12x10";

		case GL_INTERNAL_RGBA_ASTC_12x12:
			return "GL_COMPRESSED_RGBA_ASTC_12x12";

		case GL_INTERNAL_SRGB8_ALPHA8_ASTC_4x4:
			return "GL_COMPRESSED_SRGB8_ALPHA8_ASTC_4x4";

		case GL_INTERNAL_SRGB8_ALPHA8_ASTC_5x4:
			return "GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x4";

		case GL_INTERNAL_SRGB8_ALPHA8_ASTC_5x5:
			return "GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x5";

		case GL_INTERNAL_SRGB8_ALPHA8_ASTC_6x5:
			return "GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x5";

		case GL_INTERNAL_SRGB8_ALPHA8_ASTC_6x6:
			return "GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x6";

		case GL_INTERNAL_SRGB8_ALPHA8_ASTC_8x5:
			return "GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x5";

		case GL_INTERNAL_SRGB8_ALPHA8_ASTC_8x6:
			return "GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x6";

		case GL_INTERNAL_SRGB8_ALPHA8_ASTC_8x8:
			return "GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x8";

		case GL_INTERNAL_SRGB8_ALPHA8_ASTC_10x5:
			return "GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x5";

		case GL_INTERNAL_SRGB8_ALPHA8_ASTC_10x6:
			return "GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x6";

		case GL_INTERNAL_SRGB8_ALPHA8_ASTC_10x8:
			return "GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x8";

		case GL_INTERNAL_SRGB8_ALPHA8_ASTC_10x10:
			return "GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x10";

		case GL_INTERNAL_SRGB8_ALPHA8_ASTC_12x10:
			return "GL_COMPRESSED_SRGB8_ALPHA8_ASTC_12x10";

		case GL_INTERNAL_SRGB8_ALPHA8_ASTC_12x12:
			return "GL_COMPRESSED_SRGB8_ALPHA8_ASTC_12x12";


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

		case renderer::PixelFormat::eRGBA16F:
			return GL_INTERNAL_RGBA16F;

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

		case renderer::PixelFormat::eBC1_RGB:
			return GL_INTERNAL_RGB_S3TC_DXT1;

		case renderer::PixelFormat::eBC1_RGBA:
			return GL_INTERNAL_RGBA_S3TC_DXT1;

		case renderer::PixelFormat::eBC1_SRGB:
			return GL_INTERNAL_SRGB_S3TC_DXT1;

		case renderer::PixelFormat::eBC1_SRGBA:
			return GL_INTERNAL_SRGB_ALPHA_S3TC_DXT1;

		case renderer::PixelFormat::eBC2_RGBA:
			return GL_INTERNAL_RGBA_S3TC_DXT3;

		case renderer::PixelFormat::eBC2_SRGBA:
			return GL_INTERNAL_SRGB_ALPHA_S3TC_DXT3;

		case renderer::PixelFormat::eBC3_RGBA:
			return GL_INTERNAL_RGBA_S3TC_DXT5;

		case renderer::PixelFormat::eBC3_SRGBA:
			return GL_INTERNAL_SRGB_ALPHA_S3TC_DXT5;

		case renderer::PixelFormat::eBC4:
			return GL_INTERNAL_RED_RGTC1;

		case renderer::PixelFormat::eBC5_RG:
			return GL_INTERNAL_RG_RGTC2;

		case renderer::PixelFormat::eASTC_4x4_RGBA:
			return GL_INTERNAL_RGBA_ASTC_4x4;

		case renderer::PixelFormat::eASTC_4x4_SRGBA:
			return GL_INTERNAL_SRGB8_ALPHA8_ASTC_4x4;

		case renderer::PixelFormat::eASTC_5x4_RGBA:
			return GL_INTERNAL_RGBA_ASTC_5x4;

		case renderer::PixelFormat::eASTC_5x4_SRGBA:
			return GL_INTERNAL_SRGB8_ALPHA8_ASTC_5x4;

		case renderer::PixelFormat::eASTC_5x5_RGBA:
			return GL_INTERNAL_RGBA_ASTC_5x5;

		case renderer::PixelFormat::eASTC_5x5_SRGBA:
			return GL_INTERNAL_SRGB8_ALPHA8_ASTC_5x5;

		case renderer::PixelFormat::eASTC_6x5_RGBA:
			return GL_INTERNAL_RGBA_ASTC_6x5;

		case renderer::PixelFormat::eASTC_6x5_SRGBA:
			return GL_INTERNAL_SRGB8_ALPHA8_ASTC_6x5;

		case renderer::PixelFormat::eASTC_6x6_RGBA:
			return GL_INTERNAL_RGBA_ASTC_6x6;

		case renderer::PixelFormat::eASTC_6x6_SRGBA:
			return GL_INTERNAL_SRGB8_ALPHA8_ASTC_6x6;

		case renderer::PixelFormat::eASTC_8x5_RGBA:
			return GL_INTERNAL_RGBA_ASTC_8x5;

		case renderer::PixelFormat::eASTC_8x5_SRGBA:
			return GL_INTERNAL_SRGB8_ALPHA8_ASTC_8x5;

		case renderer::PixelFormat::eASTC_8x6_RGBA:
			return GL_INTERNAL_RGBA_ASTC_8x6;

		case renderer::PixelFormat::eASTC_8x6_SRGBA:
			return GL_INTERNAL_SRGB8_ALPHA8_ASTC_8x6;

		case renderer::PixelFormat::eASTC_8x8_RGBA:
			return GL_INTERNAL_RGBA_ASTC_8x8;

		case renderer::PixelFormat::eASTC_8x8_SRGBA:
			return GL_INTERNAL_SRGB8_ALPHA8_ASTC_8x8;

		case renderer::PixelFormat::eASTC_10x5_RGBA:
			return GL_INTERNAL_RGBA_ASTC_10x5;

		case renderer::PixelFormat::eASTC_10x5_SRGBA:
			return GL_INTERNAL_SRGB8_ALPHA8_ASTC_10x5;

		case renderer::PixelFormat::eASTC_10x6_RGBA:
			return GL_INTERNAL_RGBA_ASTC_10x6;

		case renderer::PixelFormat::eASTC_10x6_SRGBA:
			return GL_INTERNAL_SRGB8_ALPHA8_ASTC_10x6;

		case renderer::PixelFormat::eASTC_10x8_RGBA:
			return GL_INTERNAL_RGBA_ASTC_10x8;

		case renderer::PixelFormat::eASTC_10x8_SRGBA:
			return GL_INTERNAL_SRGB8_ALPHA8_ASTC_10x8;

		case renderer::PixelFormat::eASTC_10x10_RGBA:
			return GL_INTERNAL_RGBA_ASTC_10x10;

		case renderer::PixelFormat::eASTC_10x10_SRGBA:
			return GL_INTERNAL_SRGB8_ALPHA8_ASTC_10x10;

		case renderer::PixelFormat::eASTC_12x10_RGBA:
			return GL_INTERNAL_RGBA_ASTC_12x10;

		case renderer::PixelFormat::eASTC_12x10_SRGBA:
			return GL_INTERNAL_SRGB8_ALPHA8_ASTC_12x10;

		case renderer::PixelFormat::eASTC_12x12_RGBA:
			return GL_INTERNAL_RGBA_ASTC_12x12;

		case renderer::PixelFormat::eASTC_12x12_SRGBA:
			return GL_INTERNAL_SRGB8_ALPHA8_ASTC_12x12;

		case renderer::PixelFormat::eETC2_R8G8B8:
			return GL_INTERNAL_RGB8_ETC2;

		case renderer::PixelFormat::eETC2_R8G8B8_SRGB:
			return GL_INTERNAL_SRGB8_ETC2;

		case renderer::PixelFormat::eETC2_R8G8B8A1:
			return GL_INTERNAL_RGB8_PUNCHTHROUGH_ALPHA1_ETC2;

		case renderer::PixelFormat::eETC2_R8G8B8A1_SRGB:
			return GL_INTERNAL_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2;

		case renderer::PixelFormat::eETC2_R8G8B8A8:
			return GL_INTERNAL_RGBA8_ETC2_EAC;

		case renderer::PixelFormat::eETC2_R8G8B8A8_SRGB:
			return GL_INTERNAL_SRGB8_ALPHA8_ETC2_EAC;

		case renderer::PixelFormat::eEAC_R11U:
			return GL_INTERNAL_R11_EAC;

		case renderer::PixelFormat::eEAC_R11S:
			return GL_INTERNAL_SIGNED_R11_EAC;

		case renderer::PixelFormat::eEAC_R11G11U:
			return GL_INTERNAL_RG11_EAC;

		case renderer::PixelFormat::eEAC_R11G11S:
			return GL_INTERNAL_SIGNED_RG11_EAC;

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
		case renderer::PixelFormat::eBC4:
		case renderer::PixelFormat::eEAC_R11U:
		case renderer::PixelFormat::eEAC_R11S:
			return GL_FORMAT_R;

		case renderer::PixelFormat::eR8G8:
		case renderer::PixelFormat::eBC5_RG:
		case renderer::PixelFormat::eEAC_R11G11U:
		case renderer::PixelFormat::eEAC_R11G11S:
			return GL_FORMAT_RG;

		case renderer::PixelFormat::eR32F:
			return GL_FORMAT_R;

		case renderer::PixelFormat::eR8G8B8:
		case renderer::PixelFormat::eBC1_RGB:
		case renderer::PixelFormat::eBC1_SRGB:
		case renderer::PixelFormat::eETC2_R8G8B8:
		case renderer::PixelFormat::eETC2_R8G8B8_SRGB:
			return GL_FORMAT_RGB;

		case renderer::PixelFormat::eETC2_R8G8B8A1:
		case renderer::PixelFormat::eETC2_R8G8B8A1_SRGB:
		case renderer::PixelFormat::eBC1_RGBA:
		case renderer::PixelFormat::eBC1_SRGBA:
		case renderer::PixelFormat::eBC2_RGBA:
		case renderer::PixelFormat::eBC2_SRGBA:
		case renderer::PixelFormat::eBC3_RGBA:
		case renderer::PixelFormat::eBC3_SRGBA:
		case renderer::PixelFormat::eASTC_4x4_RGBA:
		case renderer::PixelFormat::eASTC_4x4_SRGBA:
		case renderer::PixelFormat::eASTC_5x4_RGBA:
		case renderer::PixelFormat::eASTC_5x4_SRGBA:
		case renderer::PixelFormat::eASTC_5x5_RGBA:
		case renderer::PixelFormat::eASTC_5x5_SRGBA:
		case renderer::PixelFormat::eASTC_6x5_RGBA:
		case renderer::PixelFormat::eASTC_6x5_SRGBA:
		case renderer::PixelFormat::eASTC_6x6_RGBA:
		case renderer::PixelFormat::eASTC_6x6_SRGBA:
		case renderer::PixelFormat::eASTC_8x5_RGBA:
		case renderer::PixelFormat::eASTC_8x5_SRGBA:
		case renderer::PixelFormat::eASTC_8x6_RGBA:
		case renderer::PixelFormat::eASTC_8x6_SRGBA:
		case renderer::PixelFormat::eASTC_8x8_RGBA:
		case renderer::PixelFormat::eASTC_8x8_SRGBA:
		case renderer::PixelFormat::eASTC_10x5_RGBA:
		case renderer::PixelFormat::eASTC_10x5_SRGBA:
		case renderer::PixelFormat::eASTC_10x6_RGBA:
		case renderer::PixelFormat::eASTC_10x6_SRGBA:
		case renderer::PixelFormat::eASTC_10x8_RGBA:
		case renderer::PixelFormat::eASTC_10x8_SRGBA:
		case renderer::PixelFormat::eASTC_10x10_RGBA:
		case renderer::PixelFormat::eASTC_10x10_SRGBA:
		case renderer::PixelFormat::eASTC_12x10_RGBA:
		case renderer::PixelFormat::eASTC_12x10_SRGBA:
		case renderer::PixelFormat::eASTC_12x12_RGBA:
		case renderer::PixelFormat::eASTC_12x12_SRGBA:
		case renderer::PixelFormat::eETC2_R8G8B8A8:
		case renderer::PixelFormat::eETC2_R8G8B8A8_SRGB:
			return GL_FORMAT_RGBA;

		case renderer::PixelFormat::eR8G8B8A8:
#if defined( _WIN32 ) || defined( __linux__ )
			return GL_FORMAT_ABGR;
#else
			return GL_FORMAT_RGBA;
#endif

		case renderer::PixelFormat::eB8G8R8A8:
			return GL_FORMAT_BGRA;

		case renderer::PixelFormat::eRGBA16F:
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

		case renderer::PixelFormat::eRGBA16F:
			return GL_TYPE_HF;

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

		case renderer::PixelFormat::eBC1_RGB:
		case renderer::PixelFormat::eBC1_RGBA:
		case renderer::PixelFormat::eBC1_SRGB:
		case renderer::PixelFormat::eBC1_SRGBA:
		case renderer::PixelFormat::eBC2_RGBA:
		case renderer::PixelFormat::eBC2_SRGBA:
		case renderer::PixelFormat::eBC3_RGBA:
		case renderer::PixelFormat::eBC3_SRGBA:
		case renderer::PixelFormat::eBC4:
		case renderer::PixelFormat::eBC5_RG:
		case renderer::PixelFormat::eASTC_4x4_RGBA:
		case renderer::PixelFormat::eASTC_4x4_SRGBA:
		case renderer::PixelFormat::eASTC_5x4_RGBA:
		case renderer::PixelFormat::eASTC_5x4_SRGBA:
		case renderer::PixelFormat::eASTC_5x5_RGBA:
		case renderer::PixelFormat::eASTC_5x5_SRGBA:
		case renderer::PixelFormat::eASTC_6x5_RGBA:
		case renderer::PixelFormat::eASTC_6x5_SRGBA:
		case renderer::PixelFormat::eASTC_6x6_RGBA:
		case renderer::PixelFormat::eASTC_6x6_SRGBA:
		case renderer::PixelFormat::eASTC_8x5_RGBA:
		case renderer::PixelFormat::eASTC_8x5_SRGBA:
		case renderer::PixelFormat::eASTC_8x6_RGBA:
		case renderer::PixelFormat::eASTC_8x6_SRGBA:
		case renderer::PixelFormat::eASTC_8x8_RGBA:
		case renderer::PixelFormat::eASTC_8x8_SRGBA:
		case renderer::PixelFormat::eASTC_10x5_RGBA:
		case renderer::PixelFormat::eASTC_10x5_SRGBA:
		case renderer::PixelFormat::eASTC_10x6_RGBA:
		case renderer::PixelFormat::eASTC_10x6_SRGBA:
		case renderer::PixelFormat::eASTC_10x8_RGBA:
		case renderer::PixelFormat::eASTC_10x8_SRGBA:
		case renderer::PixelFormat::eASTC_10x10_RGBA:
		case renderer::PixelFormat::eASTC_10x10_SRGBA:
		case renderer::PixelFormat::eASTC_12x10_RGBA:
		case renderer::PixelFormat::eASTC_12x10_SRGBA:
		case renderer::PixelFormat::eASTC_12x12_RGBA:
		case renderer::PixelFormat::eASTC_12x12_SRGBA:
		case renderer::PixelFormat::eETC2_R8G8B8:
		case renderer::PixelFormat::eETC2_R8G8B8_SRGB:
		case renderer::PixelFormat::eETC2_R8G8B8A1:
		case renderer::PixelFormat::eETC2_R8G8B8A1_SRGB:
		case renderer::PixelFormat::eETC2_R8G8B8A8:
		case renderer::PixelFormat::eETC2_R8G8B8A8_SRGB:
		case renderer::PixelFormat::eEAC_R11U:
		case renderer::PixelFormat::eEAC_R11S:
		case renderer::PixelFormat::eEAC_R11G11U:
		case renderer::PixelFormat::eEAC_R11G11S:
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

		case GL_INTERNAL_RGBA16F:
			return renderer::PixelFormat::eRGBA16F;

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

		case GL_INTERNAL_RGB_S3TC_DXT1:
			return renderer::PixelFormat::eBC1_RGB;

		case GL_INTERNAL_RGBA_S3TC_DXT1:
			return renderer::PixelFormat::eBC1_RGBA;

		case GL_INTERNAL_SRGB_S3TC_DXT1:
			return renderer::PixelFormat::eBC1_SRGB;

		case GL_INTERNAL_SRGB_ALPHA_S3TC_DXT1:
			return renderer::PixelFormat::eBC1_SRGBA;

		case GL_INTERNAL_RGBA_S3TC_DXT3:
			return renderer::PixelFormat::eBC2_RGBA;

		case GL_INTERNAL_SRGB_ALPHA_S3TC_DXT3:
			return renderer::PixelFormat::eBC2_SRGBA;

		case GL_INTERNAL_RGBA_S3TC_DXT5:
			return renderer::PixelFormat::eBC3_RGBA;

		case GL_INTERNAL_SRGB_ALPHA_S3TC_DXT5:
			return renderer::PixelFormat::eBC3_SRGBA;

		case GL_INTERNAL_RED_RGTC1:
			return renderer::PixelFormat::eBC4;

		case GL_INTERNAL_RG_RGTC2:
			return renderer::PixelFormat::eBC5_RG;

		case GL_INTERNAL_RGBA_ASTC_4x4:
			return renderer::PixelFormat::eASTC_4x4_RGBA;

		case GL_INTERNAL_SRGB8_ALPHA8_ASTC_4x4:
			return renderer::PixelFormat::eASTC_4x4_SRGBA;

		case GL_INTERNAL_RGBA_ASTC_5x4:
			return renderer::PixelFormat::eASTC_5x4_RGBA;

		case GL_INTERNAL_SRGB8_ALPHA8_ASTC_5x4:
			return renderer::PixelFormat::eASTC_5x4_SRGBA;

		case GL_INTERNAL_RGBA_ASTC_5x5:
			return renderer::PixelFormat::eASTC_5x5_RGBA;

		case GL_INTERNAL_SRGB8_ALPHA8_ASTC_5x5:
			return renderer::PixelFormat::eASTC_5x5_SRGBA;

		case GL_INTERNAL_RGBA_ASTC_6x5:
			return renderer::PixelFormat::eASTC_6x5_RGBA;

		case GL_INTERNAL_SRGB8_ALPHA8_ASTC_6x5:
			return renderer::PixelFormat::eASTC_6x5_SRGBA;

		case GL_INTERNAL_RGBA_ASTC_6x6:
			return renderer::PixelFormat::eASTC_6x6_RGBA;

		case GL_INTERNAL_SRGB8_ALPHA8_ASTC_6x6:
			return renderer::PixelFormat::eASTC_6x6_SRGBA;

		case GL_INTERNAL_RGBA_ASTC_8x5:
			return renderer::PixelFormat::eASTC_8x5_RGBA;

		case GL_INTERNAL_SRGB8_ALPHA8_ASTC_8x5:
			return renderer::PixelFormat::eASTC_8x5_SRGBA;

		case GL_INTERNAL_RGBA_ASTC_8x6:
			return renderer::PixelFormat::eASTC_8x6_RGBA;

		case GL_INTERNAL_SRGB8_ALPHA8_ASTC_8x6:
			return renderer::PixelFormat::eASTC_8x6_SRGBA;

		case GL_INTERNAL_RGBA_ASTC_8x8:
			return renderer::PixelFormat::eASTC_8x8_RGBA;

		case GL_INTERNAL_SRGB8_ALPHA8_ASTC_8x8:
			return renderer::PixelFormat::eASTC_8x8_SRGBA;

		case GL_INTERNAL_RGBA_ASTC_10x5:
			return renderer::PixelFormat::eASTC_10x5_RGBA;

		case GL_INTERNAL_SRGB8_ALPHA8_ASTC_10x5:
			return renderer::PixelFormat::eASTC_10x5_SRGBA;

		case GL_INTERNAL_RGBA_ASTC_10x6:
			return renderer::PixelFormat::eASTC_10x6_RGBA;

		case GL_INTERNAL_SRGB8_ALPHA8_ASTC_10x6:
			return renderer::PixelFormat::eASTC_10x6_SRGBA;

		case GL_INTERNAL_RGBA_ASTC_10x8:
			return renderer::PixelFormat::eASTC_10x8_RGBA;

		case GL_INTERNAL_SRGB8_ALPHA8_ASTC_10x8:
			return renderer::PixelFormat::eASTC_10x8_SRGBA;

		case GL_INTERNAL_RGBA_ASTC_10x10:
			return renderer::PixelFormat::eASTC_10x10_RGBA;

		case GL_INTERNAL_SRGB8_ALPHA8_ASTC_10x10:
			return renderer::PixelFormat::eASTC_10x10_SRGBA;

		case GL_INTERNAL_RGBA_ASTC_12x10:
			return renderer::PixelFormat::eASTC_12x10_RGBA;

		case GL_INTERNAL_SRGB8_ALPHA8_ASTC_12x10:
			return renderer::PixelFormat::eASTC_12x10_SRGBA;

		case GL_INTERNAL_RGBA_ASTC_12x12:
			return renderer::PixelFormat::eASTC_12x12_RGBA;

		case GL_INTERNAL_SRGB8_ALPHA8_ASTC_12x12:
			return renderer::PixelFormat::eASTC_12x12_SRGBA;

		case GL_INTERNAL_RGB8_ETC2:
			return renderer::PixelFormat::eETC2_R8G8B8;

		case GL_INTERNAL_SRGB8_ETC2:
			return renderer::PixelFormat::eETC2_R8G8B8_SRGB;

		case GL_INTERNAL_RGB8_PUNCHTHROUGH_ALPHA1_ETC2:
			return renderer::PixelFormat::eETC2_R8G8B8A1;

		case GL_INTERNAL_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2:
			return renderer::PixelFormat::eETC2_R8G8B8A1_SRGB;

		case GL_INTERNAL_RGBA8_ETC2_EAC:
			return renderer::PixelFormat::eETC2_R8G8B8A8;

		case GL_INTERNAL_SRGB8_ALPHA8_ETC2_EAC:
			return renderer::PixelFormat::eETC2_R8G8B8A8_SRGB;

		case GL_INTERNAL_R11_EAC:
			return renderer::PixelFormat::eEAC_R11U;

		case GL_INTERNAL_SIGNED_R11_EAC:
			return renderer::PixelFormat::eEAC_R11S;

		case GL_INTERNAL_RG11_EAC:
			return renderer::PixelFormat::eEAC_R11G11U;

		case GL_INTERNAL_SIGNED_RG11_EAC:
			return renderer::PixelFormat::eEAC_R11G11S;

		default:
			assert( false && "Unsupported pixel format." );
			return renderer::PixelFormat::eR8G8B8A8;
		}
	}
}
