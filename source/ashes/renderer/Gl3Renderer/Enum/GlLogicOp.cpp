#include "GlRendererPrerequisites.hpp"

namespace ashes::gl3
{
	std::string getName( GlLogicOp value )
	{
		switch ( value )
		{
		case GL_LOGIC_OP_CLEAR:
			return "GL_CLEAR";

		case GL_LOGIC_OP_AND:
			return "GL_AND";

		case GL_LOGIC_OP_AND_REVERSE:
			return "GL_AND_REVERSE";

		case GL_LOGIC_OP_COPY:
			return "GL_COPY";

		case GL_LOGIC_OP_AND_INVERTED:
			return "GL_AND_INVERTED";

		case GL_LOGIC_OP_NOOP:
			return "GL_NOOP";

		case GL_LOGIC_OP_XOR:
			return "GL_XOR";

		case GL_LOGIC_OP_OR:
			return "GL_OR";

		case GL_LOGIC_OP_NOR:
			return "GL_NOR";

		case GL_LOGIC_OP_EQUIV:
			return "GL_EQUIV";

		case GL_LOGIC_OP_INVERT:
			return "GL_INVERT";

		case GL_LOGIC_OP_OR_REVERSE:
			return "GL_OR_REVERSE";

		case GL_LOGIC_OP_COPY_INVERTED:
			return "GL_COPY_INVERTED";

		case GL_LOGIC_OP_OR_INVERTED:
			return "GL_OR_INVERTED";

		case GL_LOGIC_OP_NAND:
			return "GL_NAND";

		case GL_LOGIC_OP_SET:
			return "GL_SET";

		default:
			assert( false && "Unsupported GlLogicOp" );
			return "GlLogicOp_UNKNOWN";
		}
	}

	GlLogicOp convert( VkLogicOp const & value )
	{
		switch ( value )
		{
		case VK_LOGIC_OP_CLEAR:
			return GL_LOGIC_OP_CLEAR;

		case VK_LOGIC_OP_AND:
			return GL_LOGIC_OP_AND;

		case VK_LOGIC_OP_AND_REVERSE:
			return GL_LOGIC_OP_AND_REVERSE;

		case VK_LOGIC_OP_COPY:
			return GL_LOGIC_OP_COPY;

		case VK_LOGIC_OP_AND_INVERTED:
			return GL_LOGIC_OP_AND_INVERTED;

		case VK_LOGIC_OP_NO_OP:
			return GL_LOGIC_OP_NOOP;

		case VK_LOGIC_OP_XOR:
			return GL_LOGIC_OP_XOR;

		case VK_LOGIC_OP_OR:
			return GL_LOGIC_OP_OR;

		case VK_LOGIC_OP_NOR:
			return GL_LOGIC_OP_NOR;

		case VK_LOGIC_OP_EQUIVALENT:
			return GL_LOGIC_OP_EQUIV;

		case VK_LOGIC_OP_INVERT:
			return GL_LOGIC_OP_INVERT;

		case VK_LOGIC_OP_OR_REVERSE:
			return GL_LOGIC_OP_OR_REVERSE;

		case VK_LOGIC_OP_COPY_INVERTED:
			return GL_LOGIC_OP_COPY_INVERTED;

		case VK_LOGIC_OP_OR_INVERTED:
			return GL_LOGIC_OP_OR_INVERTED;

		case VK_LOGIC_OP_NAND:
			return GL_LOGIC_OP_NAND;

		case VK_LOGIC_OP_SET:
			return GL_LOGIC_OP_SET;

		default:
			assert( false && "Unsupported logic operator");
			return GL_LOGIC_OP_COPY;
		}
	}
}
