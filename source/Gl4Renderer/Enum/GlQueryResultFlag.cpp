#include "GlRendererPrerequisites.hpp"

namespace gl_renderer
{
	std::string getName( GlQueryResultFlag value )
	{
		switch ( value )
		{
		case 0u:
			return "0";

		case gl_renderer::GL_QUERY_RESULT:
			return "GL_QUERY_RESULT";

		case gl_renderer::GL_QUERY_RESULT_NO_WAIT:
			return "GL_QUERY_RESULT_NO_WAIT";

		default:
			assert( false && "Unsupported GlQueryResultFlag" );
			return "GlQueryResultFlag_UNKNOWN";
		}
	}

	GLuint convert( ashes::QueryResultFlags const & flags )
	{
		GLuint result{ 0 };

		if ( checkFlag( flags, ashes::QueryResultFlag::eWait ) )
		{
			return GL_QUERY_RESULT;
		}

		if ( checkFlag( flags, ashes::QueryResultFlag::eWithAvailability ) )
		{
			return GL_QUERY_RESULT_NO_WAIT;
		}

		return result;
	}
}
