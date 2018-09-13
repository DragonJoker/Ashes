#include "GlRendererPrerequisites.hpp"

namespace gl_renderer
{
	std::string getName( GlTweak value )
	{
		switch ( value )
		{
		case gl_renderer::GL_CULL_FACE:
			return "GL_CULL_FACE";

		case gl_renderer::GL_DEPTH_RANGE:
			return "GL_DEPTH_RANGE";

		case gl_renderer::GL_DEPTH_TEST:
			return "GL_DEPTH_TEST";

		case gl_renderer::GL_DEPTH_WRITEMASK:
			return "GL_DEPTH_WRITEMASK";

		case gl_renderer::GL_DEPTH_FUNC:
			return "GL_DEPTH_FUNC";

		case gl_renderer::GL_STENCIL_TEST:
			return "GL_STENCIL_TEST";

		case gl_renderer::GL_STENCIL_FRONT_FUNC:
			return "GL_STENCIL_FRONT_FUNC";

		case gl_renderer::GL_STENCIL_FRONT_VALUE_MASK:
			return "GL_STENCIL_FRONT_VALUE_MASK";

		case gl_renderer::GL_STENCIL_FRONT_FAIL:
			return "GL_STENCIL_FRONT_FAIL";

		case gl_renderer::GL_STENCIL_FRONT_PASS_DEPTH_FAIL:
			return "GL_STENCIL_FRONT_PASS_DEPTH_FAIL";

		case gl_renderer::GL_STENCIL_FRONT_PASS_DEPTH_PASS:
			return "GL_STENCIL_FRONT_PASS_DEPTH_PASS";

		case gl_renderer::GL_STENCIL_FRONT_REF:
			return "GL_STENCIL_FRONT_REF";

		case gl_renderer::GL_STENCIL_FRONT_WRITEMASK:
			return "GL_STENCIL_FRONT_WRITEMASK";

		case gl_renderer::GL_BLEND:
			return "GL_BLEND";

		case gl_renderer::GL_POLYGON_OFFSET_POINT:
			return "GL_POLYGON_OFFSET_POINT";

		case gl_renderer::GL_POLYGON_OFFSET_LINE:
			return "GL_POLYGON_OFFSET_LINE";

		case gl_renderer::GL_MULTISAMPLE:
			return "GL_MULTISAMPLE";

		case gl_renderer::GL_SAMPLE_ALPHA_TO_COVERAGE:
			return "GL_SAMPLE_ALPHA_TO_COVERAGE";

		case gl_renderer::GL_SAMPLE_ALPHA_TO_ONE:
			return "GL_SAMPLE_ALPHA_TO_ONE";

		case gl_renderer::GL_POLYGON_OFFSET_FILL:
			return "GL_POLYGON_OFFSET_FILL";

		case gl_renderer::GL_PROGRAM_POINT_SIZE:
			return "GL_PROGRAM_POINT_SIZE";

		case gl_renderer::GL_DEPTH_CLAMP:
			return "GL_DEPTH_CLAMP";

		case gl_renderer::GL_STENCIL_BACK_FUNC:
			return "GL_STENCIL_BACK_FUNC";

		case gl_renderer::GL_STENCIL_BACK_FAIL:
			return "GL_STENCIL_BACK_FAIL";

		case gl_renderer::GL_STENCIL_BACK_PASS_DEPTH_FAIL:
			return "GL_STENCIL_BACK_PASS_DEPTH_FAIL";

		case gl_renderer::GL_STENCIL_BACK_PASS_DEPTH_PASS:
			return "GL_STENCIL_BACK_PASS_DEPTH_PASS";

		case gl_renderer::GL_RASTERIZER_DISCARD:
			return "GL_RASTERIZER_DISCARD";

		case gl_renderer::GL_STENCIL_BACK_REF:
			return "GL_STENCIL_BACK_REF";

		case gl_renderer::GL_STENCIL_BACK_VALUE_MASK:
			return "GL_STENCIL_BACK_VALUE_MASK";

		case gl_renderer::GL_STENCIL_BACK_WRITEMASK:
			return "GL_STENCIL_BACK_WRITEMASK";

		case gl_renderer::GL_PRIMITIVE_RESTART:
			return "GL_PRIMITIVE_RESTART";

		case gl_renderer::GL_SAMPLE_SHADING:
			return "GL_SAMPLE_SHADING";

		default:
			assert( false && "Unsupported GlTweak" );
			return "GlTweak_UNKNOWN";
		}
	}
}
