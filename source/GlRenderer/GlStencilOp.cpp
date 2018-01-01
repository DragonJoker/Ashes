#include "GlStencilOp.hpp"

namespace gl_renderer
{
	GLenum convert( renderer::StencilOp const & value )
	{
		switch ( value )
		{
		case renderer::StencilOp::eKeep:
			return GL_KEEP;

		case renderer::StencilOp::eZero:
			return GL_ZERO;

		case renderer::StencilOp::eReplace:
			return GL_REPLACE;

		case renderer::StencilOp::eIncrementAndClamp:
			return GL_INCR;

		case renderer::StencilOp::eDecrementAndClamp:
			return GL_DECR;

		case renderer::StencilOp::eInvert:
			return GL_INVERT;

		case renderer::StencilOp::eIncrementAndWrap:
			return GL_INCR_WRAP;

		case renderer::StencilOp::eDecrementAndWrap:
			return GL_DECR_WRAP;

		default:
			assert( false && "Unsupported stencil operator");
			return GL_KEEP;
		}
	}
}
