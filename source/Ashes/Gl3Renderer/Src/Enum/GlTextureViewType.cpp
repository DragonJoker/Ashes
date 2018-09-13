#include "GlRendererPrerequisites.hpp"

namespace gl_renderer
{
	GlTextureType convert( ashes::TextureViewType const & value )
	{
		switch ( value )
		{
		case ashes::TextureViewType::e1D:
			return GL_TEXTURE_1D;

		case ashes::TextureViewType::e2D:
			return GL_TEXTURE_2D;

		case ashes::TextureViewType::e3D:
			return GL_TEXTURE_3D;

		case ashes::TextureViewType::eCube:
			return GL_TEXTURE_CUBE;

		case ashes::TextureViewType::e1DArray:
			return GL_TEXTURE_1D_ARRAY;

		case ashes::TextureViewType::e2DArray:
			return GL_TEXTURE_2D_ARRAY;

		case ashes::TextureViewType::eCubeArray:
			return GL_TEXTURE_CUBE_ARRAY;

		default:
			assert( false && "Unsupported TextureViewType" );
			return GL_TEXTURE_2D;
		}
	}
}
