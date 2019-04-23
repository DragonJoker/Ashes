#include "GlRendererPrerequisites.hpp"

namespace gl_renderer
{
	GLsizei convert( VkSampleCountFlagBits const & flags )
	{
		return GLsizei( flags );
	}
}
