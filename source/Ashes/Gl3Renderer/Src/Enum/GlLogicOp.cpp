#include "GlRendererPrerequisites.hpp"

namespace gl_renderer
{
	std::string getName( GlLogicOp value )
	{
		switch ( value )
		{
		case gl_renderer::GL_LOGIC_OP_CLEAR:
			return "GL_CLEAR";

		case gl_renderer::GL_LOGIC_OP_AND:
			return "GL_AND";

		case gl_renderer::GL_LOGIC_OP_AND_REVERSE:
			return "GL_AND_REVERSE";

		case gl_renderer::GL_LOGIC_OP_COPY:
			return "GL_COPY";

		case gl_renderer::GL_LOGIC_OP_AND_INVERTED:
			return "GL_AND_INVERTED";

		case gl_renderer::GL_LOGIC_OP_NOOP:
			return "GL_NOOP";

		case gl_renderer::GL_LOGIC_OP_XOR:
			return "GL_XOR";

		case gl_renderer::GL_LOGIC_OP_OR:
			return "GL_OR";

		case gl_renderer::GL_LOGIC_OP_NOR:
			return "GL_NOR";

		case gl_renderer::GL_LOGIC_OP_EQUIV:
			return "GL_EQUIV";

		case gl_renderer::GL_LOGIC_OP_INVERT:
			return "GL_INVERT";

		case gl_renderer::GL_LOGIC_OP_OR_REVERSE:
			return "GL_OR_REVERSE";

		case gl_renderer::GL_LOGIC_OP_COPY_INVERTED:
			return "GL_COPY_INVERTED";

		case gl_renderer::GL_LOGIC_OP_OR_INVERTED:
			return "GL_OR_INVERTED";

		case gl_renderer::GL_LOGIC_OP_NAND:
			return "GL_NAND";

		case gl_renderer::GL_LOGIC_OP_SET:
			return "GL_SET";

		default:
			assert( false && "Unsupported GlLogicOp" );
			return "GlLogicOp_UNKNOWN";
		}
	}

	GlLogicOp convert( ashes::LogicOp const & value )
	{
		switch ( value )
		{
		case ashes::LogicOp::eClear:
			return GL_LOGIC_OP_CLEAR;

		case ashes::LogicOp::eAnd:
			return GL_LOGIC_OP_AND;

		case ashes::LogicOp::eAndReverse:
			return GL_LOGIC_OP_AND_REVERSE;

		case ashes::LogicOp::eCopy:
			return GL_LOGIC_OP_COPY;

		case ashes::LogicOp::eAndInverted:
			return GL_LOGIC_OP_AND_INVERTED;

		case ashes::LogicOp::eNoOp:
			return GL_LOGIC_OP_NOOP;

		case ashes::LogicOp::eXor:
			return GL_LOGIC_OP_XOR;

		case ashes::LogicOp::eOr:
			return GL_LOGIC_OP_OR;

		case ashes::LogicOp::eNor:
			return GL_LOGIC_OP_NOR;

		case ashes::LogicOp::eEquivalent:
			return GL_LOGIC_OP_EQUIV;

		case ashes::LogicOp::eInvert:
			return GL_LOGIC_OP_INVERT;

		case ashes::LogicOp::eOrReverse:
			return GL_LOGIC_OP_OR_REVERSE;

		case ashes::LogicOp::eCopyInverted:
			return GL_LOGIC_OP_COPY_INVERTED;

		case ashes::LogicOp::eOrInverted:
			return GL_LOGIC_OP_OR_INVERTED;

		case ashes::LogicOp::eNand:
			return GL_LOGIC_OP_NAND;

		case ashes::LogicOp::eSet:
			return GL_LOGIC_OP_SET;

		default:
			assert( false && "Unsupported logic operator");
			return GL_LOGIC_OP_COPY;
		}
	}
}
