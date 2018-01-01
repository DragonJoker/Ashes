#include "GlLogicOp.hpp"

namespace gl_renderer
{
	GLenum convert( renderer::LogicOp const & value )
	{
		switch ( value )
		{
		case renderer::LogicOp::eClear:
			return GL_CLEAR;

		case renderer::LogicOp::eAnd:
			return GL_AND;

		case renderer::LogicOp::eAndReverse:
			return GL_AND_REVERSE;

		case renderer::LogicOp::eCopy:
			return GL_COPY;

		case renderer::LogicOp::eAndInverted:
			return GL_AND_INVERTED;

		case renderer::LogicOp::eNoOp:
			return GL_NOOP;

		case renderer::LogicOp::eXor:
			return GL_XOR;

		case renderer::LogicOp::eOr:
			return GL_OR;

		case renderer::LogicOp::eNor:
			return GL_NOR;

		case renderer::LogicOp::eEquivalent:
			return GL_EQUIV;

		case renderer::LogicOp::eInvert:
			return GL_INVERT;

		case renderer::LogicOp::eOrReverse:
			return GL_OR_REVERSE;

		case renderer::LogicOp::eCopyInverted:
			return GL_COPY_INVERTED;

		case renderer::LogicOp::eOrInverted:
			return GL_OR_INVERTED;

		case renderer::LogicOp::eNand:
			return GL_NAND;

		case renderer::LogicOp::eSet:
			return GL_SET;

		default:
			assert( false && "Unsupported logic operator");
			return GL_COPY;
		}
	}
}
