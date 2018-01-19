#include "GlRendererPrerequisites.hpp"

namespace gl_renderer
{
	GLenum convert( renderer::BlendOp const & value )
	{
		switch ( value )
		{
		case renderer::BlendOp::eAdd:
			return GL_FUNC_ADD;

		case renderer::BlendOp::eSubtract:
			return GL_FUNC_SUBTRACT;

		case renderer::BlendOp::eReverseSubtract:
			return GL_FUNC_REVERSE_SUBTRACT;

		case renderer::BlendOp::eMin:
			return GL_MIN;

		case renderer::BlendOp::eMax:
			return GL_MAX;

		default:
			assert( false && "Unsupported blend operator" );
			return GL_ADD;
		}
	}
}
