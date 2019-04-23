#include "GlRendererPrerequisites.hpp"

namespace gl_renderer
{
	std::string getName( GlQueryType value )
	{
		switch ( value )
		{
		case gl_renderer::GL_QUERY_TYPE_SAMPLES_PASSED:
			return "GL_SAMPLES_PASSED";

		case gl_renderer::GL_QUERY_TYPE_TIMESTAMP:
			return "GL_TIMESTAMP";

		case gl_renderer::GL_QUERY_TYPE_PRIMITIVES_GENERATED:
			return "GL_PRIMITIVES_GENERATED";

		default:
			assert( false && "Unsupported GlQueryType" );
			return "GlQueryType_UNKNOWN";
		}
	}

	GlQueryType convert( VkQueryType const & value )
	{
		switch ( value )
		{
		case VK_QUERY_TYPE_OCCLUSION:
			return GL_QUERY_TYPE_SAMPLES_PASSED;

		case VK_QUERY_TYPE_TIMESTAMP:
			return GL_QUERY_TYPE_TIMESTAMP;

		case VK_QUERY_TYPE_PIPELINE_STATISTICS:
			return GL_QUERY_TYPE_PRIMITIVES_GENERATED;

		default:
			assert( false && "Unsupported query type" );
			return GL_QUERY_TYPE_TIMESTAMP;
		}
	}
}
