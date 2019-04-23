#include "GlRendererPrerequisites.hpp"

namespace gl_renderer
{
	std::string getName( GlStencilOp value )
	{
		switch ( value )
		{
		case gl_renderer::GL_STENCIL_OP_ZERO:
			return "GL_ZERO";

		case gl_renderer::GL_STENCIL_OP_KEEP:
			return "GL_KEEP";

		case gl_renderer::GL_STENCIL_OP_REPLACE:
			return "GL_REPLACE";

		case gl_renderer::GL_STENCIL_OP_INCR:
			return "GL_INCR";

		case gl_renderer::GL_STENCIL_OP_DECR:
			return "GL_DECR";

		case gl_renderer::GL_STENCIL_OP_INVERT:
			return "GL_INVERT";

		case gl_renderer::GL_STENCIL_OP_INCR_WRAP:
			return "GL_INCR_WRAP";

		case gl_renderer::GL_STENCIL_OP_DECR_WRAP:
			return "GL_DECR_WRAP";

		default:
			assert( false && "Unsupported GlStencilOp" );
			return "GlStencilOp_UNKNOWN";
		}
	}

	GlStencilOp convert( VkStencilOp const & value )
	{
		switch ( value )
		{
		case VK_STENCIL_OP_KEEP:
			return GL_STENCIL_OP_KEEP;

		case VK_STENCIL_OP_ZERO:
			return GL_STENCIL_OP_ZERO;

		case VK_STENCIL_OP_REPLACE:
			return GL_STENCIL_OP_REPLACE;

		case VK_STENCIL_OP_INCREMENT_AND_CLAMP:
			return GL_STENCIL_OP_INCR;

		case VK_STENCIL_OP_DECREMENT_AND_CLAMP:
			return GL_STENCIL_OP_DECR;

		case VK_STENCIL_OP_INVERT:
			return GL_STENCIL_OP_INVERT;

		case VK_STENCIL_OP_INCREMENT_AND_WRAP:
			return GL_STENCIL_OP_INCR_WRAP;

		case VK_STENCIL_OP_DECREMENT_AND_WRAP:
			return GL_STENCIL_OP_DECR_WRAP;

		default:
			assert( false && "Unsupported stencil operator");
			return GL_STENCIL_OP_KEEP;
		}
	}
}
