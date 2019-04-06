#include "GlRendererPrerequisites.hpp"

namespace gl_renderer
{
	GlTextureType convert( VkImageViewType const & value )
	{
		switch ( value )
		{
		case VK_IMAGE_VIEW_TYPE_1D:
			return GL_TEXTURE_1D;

		case VK_IMAGE_VIEW_TYPE_2D:
			return GL_TEXTURE_2D;

		case VK_IMAGE_VIEW_TYPE_3D:
			return GL_TEXTURE_3D;

		case VK_IMAGE_VIEW_TYPE_Cube:
			return GL_TEXTURE_CUBE;

		case VK_IMAGE_VIEW_TYPE_1DArray:
			return GL_TEXTURE_1D_ARRAY;

		case VK_IMAGE_VIEW_TYPE_2DArray:
			return GL_TEXTURE_2D_ARRAY;

		case VK_IMAGE_VIEW_TYPE_CubeArray:
			return GL_TEXTURE_CUBE_ARRAY;

		default:
			assert( false && "Unsupported ImageViewType" );
			return GL_TEXTURE_2D;
		}
	}

	GlTextureType convert( VkImageViewType const & mode
		, uint32_t layers )
	{
		switch ( mode )
		{
		case VK_IMAGE_VIEW_TYPE_1D:
			return layers > 1u
				? GL_TEXTURE_1D_ARRAY
				: GL_TEXTURE_1D;

		case VK_IMAGE_VIEW_TYPE_2D:
			return layers > 1u
				? GL_TEXTURE_2D_ARRAY
				: GL_TEXTURE_2D;

		case VK_IMAGE_VIEW_TYPE_3D:
			return GL_TEXTURE_3D;

		case VK_IMAGE_VIEW_TYPE_Cube:
			return layers > 6u
				? GL_TEXTURE_CUBE_ARRAY
				: GL_TEXTURE_CUBE;

		case VK_IMAGE_VIEW_TYPE_1DArray:
			return GL_TEXTURE_1D_ARRAY;

		case VK_IMAGE_VIEW_TYPE_2DArray:
			return GL_TEXTURE_2D_ARRAY;

		case VK_IMAGE_VIEW_TYPE_CubeArray:
			return GL_TEXTURE_CUBE_ARRAY;

		default:
			assert( false && "Unsupported ImageViewType" );
			return GL_TEXTURE_2D;
		}
	}
}
