#include "GlRendererPrerequisites.hpp"

namespace gl_renderer
{
	GlTextureType convert( ashes::ImageViewType const & value )
	{
		switch ( value )
		{
		case ashes::ImageViewType::e1D:
			return GL_TEXTURE_1D;

		case ashes::ImageViewType::e2D:
			return GL_TEXTURE_2D;

		case ashes::ImageViewType::e3D:
			return GL_TEXTURE_3D;

		case ashes::ImageViewType::eCube:
			return GL_TEXTURE_CUBE;

		case ashes::ImageViewType::e1DArray:
			return GL_TEXTURE_1D_ARRAY;

		case ashes::ImageViewType::e2DArray:
			return GL_TEXTURE_2D_ARRAY;

		case ashes::ImageViewType::eCubeArray:
			return GL_TEXTURE_CUBE_ARRAY;

		default:
			assert( false && "Unsupported ImageViewType" );
			return GL_TEXTURE_2D;
		}
	}

	GlTextureType convert( ashes::ImageViewType const & mode
		, uint32_t layers )
	{
		switch ( mode )
		{
		case ashes::ImageViewType::e1D:
			return layers > 1u
				? GL_TEXTURE_1D_ARRAY
				: GL_TEXTURE_1D;

		case ashes::ImageViewType::e2D:
			return layers > 1u
				? GL_TEXTURE_2D_ARRAY
				: GL_TEXTURE_2D;

		case ashes::ImageViewType::e3D:
			return GL_TEXTURE_3D;

		case ashes::ImageViewType::eCube:
			return layers > 6u
				? GL_TEXTURE_CUBE_ARRAY
				: GL_TEXTURE_CUBE;

		case ashes::ImageViewType::e1DArray:
			return GL_TEXTURE_1D_ARRAY;

		case ashes::ImageViewType::e2DArray:
			return GL_TEXTURE_2D_ARRAY;

		case ashes::ImageViewType::eCubeArray:
			return GL_TEXTURE_CUBE_ARRAY;

		default:
			assert( false && "Unsupported ImageViewType" );
			return GL_TEXTURE_2D;
		}
	}
}
