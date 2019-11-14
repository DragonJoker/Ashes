#include "GlRendererPrerequisites.hpp"

namespace ashes::gl3
{
	std::string getName( GlBlendOp value )
	{
		switch ( value )
		{
		case GL_BLEND_OP_ADD:
			return "GL_ADD";

		case GL_BLEND_OP_MIN:
			return "GL_MIN";

		case GL_BLEND_OP_MAX:
			return "GL_MAX";

		case GL_BLEND_OP_SUBTRACT:
			return "GL_SUBTRACT";

		case GL_BLEND_OP_REVERSE_SUBTRACT:
			return "GL_REVERSE_SUBTRACT";

		default:
			assert( false && "Unsupported GlBlendOp" );
			return "GlBlendOp_UNKNOWN";
		}
	}

	GlBlendOp convert( VkBlendOp const & value )
	{
		switch ( value )
		{
		case VK_BLEND_OP_ADD:
			return GL_BLEND_OP_ADD;

		case VK_BLEND_OP_SUBTRACT:
			return GL_BLEND_OP_SUBTRACT;

		case VK_BLEND_OP_REVERSE_SUBTRACT:
			return GL_BLEND_OP_REVERSE_SUBTRACT;

		case VK_BLEND_OP_MIN:
			return GL_BLEND_OP_MIN;

		case VK_BLEND_OP_MAX:
			return GL_BLEND_OP_MAX;

		default:
			assert( false && "Unsupported GlBlendOp" );
			return GL_BLEND_OP_ADD;
		}
	}
}
