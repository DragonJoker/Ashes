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

		case gl_renderer::GL_TEXTURE_1D_ARRAY:
			return "GL_TEXTURE_1D_ARRAY";

		case gl_renderer::GL_TEXTURE_2D_ARRAY:
			return "GL_TEXTURE_2D_ARRAY";

		case gl_renderer::GL_TEXTURE_2D_MULTISAMPLE:
			return "GL_TEXTURE_2D_MULTISAMPLE";

		case gl_renderer::GL_TEXTURE_2D_MULTISAMPLE_ARRAY:
			return "GL_TEXTURE_2D_MULTISAMPLE_ARRAY";

		default:
			assert( false && "Unsupported GlTextureType" );
			return "GlTextureType_UNKNOWN";
		}
	}

	GlTextureType convert( VkImageType const & value
		, uint32_t layerCount )
	{
		switch ( value )
		{
		case VK_IMAGE_TYPE_1D:
			if ( layerCount > 1 )
			{
				return GL_TEXTURE_1D_ARRAY;
			}
			return GL_TEXTURE_1D;

		case VK_IMAGE_TYPE_2D:
			if ( layerCount > 1 )
			{
				return GL_TEXTURE_2D_ARRAY;
			}
			return GL_TEXTURE_2D;

		case VK_IMAGE_TYPE_3D:
			return GL_TEXTURE_3D;

		default:
			assert( false && "Unsupported texture type" );
			return GL_TEXTURE_2D;
		}
	}
}
