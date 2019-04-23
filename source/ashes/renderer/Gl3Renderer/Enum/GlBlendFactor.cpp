#include "GlRendererPrerequisites.hpp"

namespace gl_renderer
{
	std::string getName( GlBlendFactor value )
	{
		switch ( value )
		{
		case gl_renderer::GL_BLEND_FACTOR_ZERO:
			return "GL_ZERO";

		case gl_renderer::GL_BLEND_FACTOR_ONE:
			return "GL_ONE";

		case gl_renderer::GL_BLEND_FACTOR_SRC_COLOR:
			return "GL_SRC_COLOR";

		case gl_renderer::GL_BLEND_FACTOR_ONE_MINUS_SRC_COLOR:
			return "GL_ONE_MINUS_SRC_COLOR";

		case gl_renderer::GL_BLEND_FACTOR_SRC_ALPHA:
			return "GL_SRC_ALPHA";

		case gl_renderer::GL_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA:
			return "GL_ONE_MINUS_SRC_ALPHA";

		case gl_renderer::GL_BLEND_FACTOR_DST_ALPHA:
			return "GL_DST_ALPHA";

		case gl_renderer::GL_BLEND_FACTOR_ONE_MINUS_DST_ALPHA:
			return "GL_ONE_MINUS_DST_ALPHA";

		case gl_renderer::GL_BLEND_FACTOR_DST_COLOR:
			return "GL_DST_COLOR";

		case gl_renderer::GL_BLEND_FACTOR_ONE_MINUS_DST_COLOR:
			return "GL_ONE_MINUS_DST_COLOR";

		case gl_renderer::GL_BLEND_FACTOR_SRC_ALPHA_SATURATE:
			return "GL_SRC_ALPHA_SATURATE";

		case gl_renderer::GL_BLEND_FACTOR_CONSTANT_COLOR:
			return "GL_CONSTANT_COLOR";

		case gl_renderer::GL_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR:
			return "GL_ONE_MINUS_CONSTANT_COLOR";

		case gl_renderer::GL_BLEND_FACTOR_CONSTANT_ALPHA:
			return "GL_CONSTANT_ALPHA";

		case gl_renderer::GL_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA:
			return "GL_ONE_MINUS_CONSTANT_ALPHA";

		case gl_renderer::GL_BLEND_FACTOR_SRC1_ALPHA:
			return "GL_SRC1_ALPHA";

		case gl_renderer::GL_BLEND_FACTOR_SRC1_COLOR:
			return "GL_SRC1_COLOR";

		case gl_renderer::GL_BLEND_FACTOR_ONE_MINUS_SRC1_COLOR:
			return "GL_ONE_MINUS_SRC1_COLOR";

		case gl_renderer::GL_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA:
			return "GL_ONE_MINUS_SRC1_ALPHA";

		default:
			assert( false && "Unsupported GlBlendFactor" );
			return "GlBlendFactor_UNKNOWN";
		}
	}

	GlBlendFactor convert( VkBlendFactor const & value )
	{
		switch ( value )
		{
		case VK_BLEND_FACTOR_ZERO:
			return GL_BLEND_FACTOR_ZERO;

		case VK_BLEND_FACTOR_ONE:
			return GL_BLEND_FACTOR_ONE;

		case VK_BLEND_FACTOR_SRC_COLOR:
			return GL_BLEND_FACTOR_SRC_COLOR;

		case VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR:
			return GL_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;

		case VK_BLEND_FACTOR_DST_COLOR:
			return GL_BLEND_FACTOR_DST_COLOR;

		case VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR:
			return GL_BLEND_FACTOR_ONE_MINUS_DST_COLOR;

		case VK_BLEND_FACTOR_SRC_ALPHA:
			return GL_BLEND_FACTOR_SRC_ALPHA;

		case VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA:
			return GL_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;

		case VK_BLEND_FACTOR_DST_ALPHA:
			return GL_BLEND_FACTOR_DST_ALPHA;

		case VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA:
			return GL_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;

		case VK_BLEND_FACTOR_CONSTANT_COLOR:
			return GL_BLEND_FACTOR_CONSTANT_COLOR;

		case VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR:
			return GL_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR;

		case VK_BLEND_FACTOR_CONSTANT_ALPHA:
			return GL_BLEND_FACTOR_CONSTANT_ALPHA;

		case VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA:
			return GL_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA;

		case VK_BLEND_FACTOR_SRC1_COLOR:
			return GL_BLEND_FACTOR_SRC1_COLOR;

		case VK_BLEND_FACTOR_ONE_MINUS_SRC1_COLOR:
			return GL_BLEND_FACTOR_ONE_MINUS_SRC1_COLOR;

		case VK_BLEND_FACTOR_SRC1_ALPHA:
			return GL_BLEND_FACTOR_SRC1_ALPHA;

		case VK_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA:
			return GL_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA;

		default:
			assert( false && "Unsupported blend factor" );
			return GL_BLEND_FACTOR_ONE;
		}
	}
}
