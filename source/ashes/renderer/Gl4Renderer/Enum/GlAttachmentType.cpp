#include "GlRendererPrerequisites.hpp"

namespace ashes::gl4
{
	std::string getName( GlAttachmentType value )
	{
		switch ( value )
		{
		case GL_ATTACHMENT_TYPE_COLOR:
			return "GL_COLOR";

		case GL_ATTACHMENT_TYPE_DEPTH:
			return "GL_DEPTH";

		case GL_ATTACHMENT_TYPE_STENCIL:
			return "GL_STENCIL";

		case GL_ATTACHMENT_TYPE_DEPTH_STENCIL:
			return "GL_DEPTH_STENCIL";

		default:
			assert( false && "Unsupported GlAttachmentType" );
			return "GlAttachmentType_UNKNOWN";
		}
	}
}
