#include "GlRendererPrerequisites.hpp"

namespace ashes::gl
{
	std::string getName( GlStencilOp value )
	{
		switch ( value )
		{
		case GL_STENCIL_OP_ZERO:
			return "GL_ZERO";

		case GL_STENCIL_OP_KEEP:
			return "GL_KEEP";

		case GL_STENCIL_OP_REPLACE:
			return "GL_REPLACE";

		case GL_STENCIL_OP_INCR:
			return "GL_INCR";

		case GL_STENCIL_OP_DECR:
			return "GL_DECR";

		case GL_STENCIL_OP_INVERT:
			return "GL_INVERT";

		case GL_STENCIL_OP_INCR_WRAP:
			return "GL_INCR_WRAP";

		case GL_STENCIL_OP_DECR_WRAP:
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
