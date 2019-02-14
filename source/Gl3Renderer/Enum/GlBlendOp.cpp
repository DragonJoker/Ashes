#include "GlRendererPrerequisites.hpp"

namespace gl_renderer
{
	std::string getName( GlBlendOp value )
	{
		switch ( value )
		{
		case gl_renderer::GL_BLEND_OP_ADD:
			return "GL_ADD";

		case gl_renderer::GL_BLEND_OP_MIN:
			return "GL_MIN";

		case gl_renderer::GL_BLEND_OP_MAX:
			return "GL_MAX";

		case gl_renderer::GL_BLEND_OP_SUBTRACT:
			return "GL_SUBTRACT";

		case gl_renderer::GL_BLEND_OP_REVERSE_SUBTRACT:
			return "GL_REVERSE_SUBTRACT";

		default:
			assert( false && "Unsupported GlBlendOp" );
			return "GlBlendOp_UNKNOWN";
		}
	}

	GlBlendOp convert( ashes::BlendOp const & value )
	{
		switch ( value )
		{
		case ashes::BlendOp::eAdd:
			return GL_BLEND_OP_ADD;

		case ashes::BlendOp::eSubtract:
			return GL_BLEND_OP_SUBTRACT;

		case ashes::BlendOp::eReverseSubtract:
			return GL_BLEND_OP_REVERSE_SUBTRACT;

		case ashes::BlendOp::eMin:
			return GL_BLEND_OP_MIN;

		case ashes::BlendOp::eMax:
			return GL_BLEND_OP_MAX;

		default:
			assert( false && "Unsupported GlBlendOp" );
			return GL_BLEND_OP_ADD;
		}
	}
}
