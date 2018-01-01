#include "GlCompareOp.hpp"

namespace gl_renderer
{
	GLenum convert( renderer::CompareOp const & value )
	{
		switch ( value )
		{
		case renderer::CompareOp::eNever:
			return GL_NEVER;

		case renderer::CompareOp::eLess:
			return GL_LESS;

		case renderer::CompareOp::eEqual:
			return GL_EQUAL;

		case renderer::CompareOp::eLessEqual:
			return GL_LEQUAL;

		case renderer::CompareOp::eGreater:
			return GL_GREATER;

		case renderer::CompareOp::eNotEqual:
			return GL_NOTEQUAL;

		case renderer::CompareOp::eGreaterEqual:
			return GL_GEQUAL;

		case renderer::CompareOp::eAlways:
			return GL_ALWAYS;

		default:
			assert( false && "Unsupported comparison operator" );
			return GL_ALWAYS;
		}
	}
}
