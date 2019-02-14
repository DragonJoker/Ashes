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

	GlStencilOp convert( ashes::StencilOp const & value )
	{
		switch ( value )
		{
		case ashes::StencilOp::eKeep:
			return GL_STENCIL_OP_KEEP;

		case ashes::StencilOp::eZero:
			return GL_STENCIL_OP_ZERO;

		case ashes::StencilOp::eReplace:
			return GL_STENCIL_OP_REPLACE;

		case ashes::StencilOp::eIncrementAndClamp:
			return GL_STENCIL_OP_INCR;

		case ashes::StencilOp::eDecrementAndClamp:
			return GL_STENCIL_OP_DECR;

		case ashes::StencilOp::eInvert:
			return GL_STENCIL_OP_INVERT;

		case ashes::StencilOp::eIncrementAndWrap:
			return GL_STENCIL_OP_INCR_WRAP;

		case ashes::StencilOp::eDecrementAndWrap:
			return GL_STENCIL_OP_DECR_WRAP;

		default:
			assert( false && "Unsupported stencil operator");
			return GL_STENCIL_OP_KEEP;
		}
	}
}
