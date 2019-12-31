#include "GlRendererPrerequisites.hpp"

namespace ashes::gl
{
	std::string getName( GlCullMode value )
	{
		switch ( value )
		{
		case GL_CULL_MODE_NONE:
			return "GL_NONE";

		case GL_CULL_MODE_FRONT:
			return "GL_FRONT";

		case GL_CULL_MODE_BACK:
			return "GL_BACK";

		case GL_CULL_MODE_FRONT_AND_BACK:
			return "GL_FRONT_AND_BACK";

		default:
			assert( false && "Unsupported GlCullMode" );
			return "GlCullMode_UNKNOWN";
		}
	}

	GlCullMode convertCullModeFlags( VkCullModeFlags flags )
	{
		GlCullMode result{ GL_CULL_MODE_NONE };

		if ( ashes::checkFlag( flags, VK_CULL_MODE_FRONT_AND_BACK ) )
		{
			result = GL_CULL_MODE_FRONT_AND_BACK;
		}
		else if ( ashes::checkFlag( flags, VK_CULL_MODE_FRONT_BIT ) )
		{
			result = GL_CULL_MODE_FRONT;
		}
		else if ( ashes::checkFlag( flags, VK_CULL_MODE_BACK_BIT ) )
		{
			result = GL_CULL_MODE_BACK;
		}

		return result;
	}
}
