#include "GlRendererPrerequisites.hpp"

namespace ashes::gl
{
	std::string getName( GlFenceWaitResult value )
	{
		switch ( value )
		{
		case GL_WAIT_RESULT_SYNC_STATUS:
			return "GL_SYNC_STATUS";
		case GL_WAIT_RESULT_UNSIGNALED:
			return "GL_UNSIGNALED";
		case GL_WAIT_RESULT_SIGNALED:
			return "GL_SIGNALED";
		case GL_WAIT_RESULT_ALREADY_SIGNALED:
			return "GL_ALREADY_SIGNALED";
		case GL_WAIT_RESULT_CONDITION_SATISFIED:
			return "GL_CONDITION_SATISFIED";
		case GL_WAIT_RESULT_TIMEOUT_EXPIRED:
			return "GL_TIMEOUT_EXPIRED";
		default:
			assert( false && "Unsupported GlFenceWaitResult" );
			return "GlFenceWaitResult_UNKNOWN";
		}
	}
}
