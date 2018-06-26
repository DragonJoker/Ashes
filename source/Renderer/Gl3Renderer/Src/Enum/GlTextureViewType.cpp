#include "GlRendererPrerequisites.hpp"

namespace gl_renderer
{
	GlTextureType convert( renderer::TextureViewType const & value )
	{
		switch ( value )
		{
		case renderer::TextureViewType::e1D:
			return GL_TEXTURE_1D;

		case renderer::TextureViewType::e2D:
			return GL_TEXTURE_2D;

		case renderer::TextureViewType::e3D:
			return GL_TEXTURE_3D;

		case renderer::TextureViewType::eCube:
			return GL_TEXTURE_CUBE;

		case renderer::TextureViewType::e1DArray:
			return GL_TEXTURE_1D_ARRAY;

		case renderer::TextureViewType::e2DArray:
			return GL_TEXTURE_2D_ARRAY;

		case renderer::TextureViewType::eCubeArray:
			return GL_TEXTURE_CUBE_ARRAY;

		default:
			assert( false && "Unsupported TextureViewType" );
			return GL_TEXTURE_2D;
		}
	}
}
