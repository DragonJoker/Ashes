#include "GlRendererPrerequisites.hpp"

namespace gl_renderer
{
	std::string getName( GlTextureType value )
	{
		switch ( value )
		{
		case gl_renderer::GL_TEXTURE_1D:
			return "GL_TEXTURE_1D";

		case gl_renderer::GL_TEXTURE_2D:
			return "GL_TEXTURE_2D";

		case gl_renderer::GL_TEXTURE_3D:
			return "GL_TEXTURE_3D";

		case gl_renderer::GL_TEXTURE_CUBE:
			return "GL_TEXTURE_CUBE_MAP";

		case gl_renderer::GL_TEXTURE_1D_ARRAY:
			return "GL_TEXTURE_1D_ARRAY";

		case gl_renderer::GL_TEXTURE_2D_ARRAY:
			return "GL_TEXTURE_2D_ARRAY";

		case gl_renderer::GL_TEXTURE_CUBE_ARRAY:
			return "GL_TEXTURE_CUBE_MAP_ARRAY";

		case gl_renderer::GL_TEXTURE_2D_MULTISAMPLE:
			return "GL_TEXTURE_2D_MULTISAMPLE";

		case gl_renderer::GL_TEXTURE_2D_MULTISAMPLE_ARRAY:
			return "GL_TEXTURE_2D_MULTISAMPLE_ARRAY";

		case gl_renderer::GL_TEXTURE_CUBE_POSITIVE_X:
			return "GL_TEXTURE_VIEW_CUBE_MAP_POSITIVE_X";
			
		case gl_renderer::GL_TEXTURE_CUBE_NEGATIVE_X:
			return "GL_TEXTURE_VIEW_CUBE_MAP_NEGATIVE_X";

		case gl_renderer::GL_TEXTURE_CUBE_POSITIVE_Y:
			return "GL_TEXTURE_VIEW_CUBE_MAP_POSITIVE_Y";

		case gl_renderer::GL_TEXTURE_CUBE_NEGATIVE_Y:
			return "GL_TEXTURE_VIEW_CUBE_MAP_NEGATIVE_Y";

		case gl_renderer::GL_TEXTURE_CUBE_POSITIVE_Z:
			return "GL_TEXTURE_VIEW_CUBE_MAP_POSITIVE_Z";

		case gl_renderer::GL_TEXTURE_CUBE_NEGATIVE_Z:
			return "GL_TEXTURE_VIEW_CUBE_MAP_NEGATIVE_Z";

		default:
			assert( false && "Unsupported GlTextureType" );
			return "GlTextureType_UNKNOWN";
		}
	}

	GlTextureType convert( ashes::ImageType const & value
		, uint32_t layerCount
		, ashes::ImageCreateFlags flags )
	{
		switch ( value )
		{
		case ashes::ImageType::e1D:
			if ( layerCount > 1 )
			{
				return GL_TEXTURE_1D_ARRAY;
			}
			return GL_TEXTURE_1D;

		case ashes::ImageType::e2D:
			if ( layerCount > 1 )
			{
				if ( checkFlag( flags, ashes::ImageCreateFlag::eCubeCompatible ) )
				{
					assert( ( layerCount % 6 ) == 0 );

					if ( layerCount > 6 )
					{
						return GL_TEXTURE_CUBE_ARRAY;
					}

					return GL_TEXTURE_CUBE;
				}

				return GL_TEXTURE_2D_ARRAY;
			}
			return GL_TEXTURE_2D;

		case ashes::ImageType::e3D:
			return GL_TEXTURE_3D;

		default:
			assert( false && "Unsupported texture type" );
			return GL_TEXTURE_2D;
		}
	}
}
