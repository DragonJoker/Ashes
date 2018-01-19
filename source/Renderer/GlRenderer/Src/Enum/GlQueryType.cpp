#include "GlRendererPrerequisites.hpp"

namespace gl_renderer
{
	GLenum convert( renderer::QueryType const & value )
	{
		switch ( value )
		{
		case renderer::QueryType::eOcclusion:
			return GL_SAMPLES_PASSED;

		case renderer::QueryType::eTimestamp:
			return GL_TIMESTAMP;

		case renderer::QueryType::ePipelineStatistics:
			return GL_PRIMITIVES_GENERATED;

		default:
			assert( false && "Unsupported query type" );
			return GL_TIMESTAMP;
		}
	}
}
