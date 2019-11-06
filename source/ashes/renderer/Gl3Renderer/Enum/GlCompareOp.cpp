#include "GlRendererPrerequisites.hpp"

namespace ashes::gl3
{
	std::string getName( GlCompareOp value )
	{
		switch ( value )
		{
		case GL_COMPARE_OP_NEVER:
			return "GL_NEVER";

		case GL_COMPARE_OP_LESS:
			return "GL_LESS";

		case GL_COMPARE_OP_EQUAL:
			return "GL_EQUAL";

		case GL_COMPARE_OP_LEQUAL:
			return "GL_LEQUAL";

		case GL_COMPARE_OP_GREATER:
			return "GL_GREATER";

		case GL_COMPARE_OP_NOTEQUAL:
			return "GL_NOTEQUAL";

		case GL_COMPARE_OP_GEQUAL:
			return "GL_GEQUAL";

		case GL_COMPARE_OP_ALWAYS:
			return "GL_ALWAYS";

		default:
			assert( false && "Unsupported GlCompareOp" );
			return "GlCompareOp_UNKNOWN";
		}
	}

	GlCompareOp convert( VkCompareOp const & value )
	{
		switch ( value )
		{
		case VK_COMPARE_OP_NEVER:
			return GL_COMPARE_OP_NEVER;

		case VK_COMPARE_OP_LESS:
			return GL_COMPARE_OP_LESS;

		case VK_COMPARE_OP_EQUAL:
			return GL_COMPARE_OP_EQUAL;

		case VK_COMPARE_OP_LESS_OR_EQUAL:
			return GL_COMPARE_OP_LEQUAL;

		case VK_COMPARE_OP_GREATER:
			return GL_COMPARE_OP_GREATER;

		case VK_COMPARE_OP_NOT_EQUAL:
			return GL_COMPARE_OP_NOTEQUAL;

		case VK_COMPARE_OP_GREATER_OR_EQUAL:
			return GL_COMPARE_OP_GEQUAL;

		case VK_COMPARE_OP_ALWAYS:
			return GL_COMPARE_OP_ALWAYS;

		default:
			assert( false && "Unsupported comparison operator" );
			return GL_COMPARE_OP_ALWAYS;
		}
	}
}
