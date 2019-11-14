#include "GlRendererPrerequisites.hpp"

namespace ashes::gl4
{
	std::string getName( GlImageAspectFlags value )
	{
		std::string result;
		std::string sep;

		if ( ashes::checkFlag( value, GlImageAspectFlag::GL_COLOR_BUFFER_BIT ) )
		{
			result += sep + "GL_COLOR_BUFFER_BIT";
			sep = " | ";
		}

		if ( ashes::checkFlag( value, GlImageAspectFlag::GL_DEPTH_BUFFER_BIT ) )
		{
			result += sep + "GL_DEPTH_BUFFER_BIT";
			sep = " | ";
		}

		if ( ashes::checkFlag( value, GlImageAspectFlag::GL_STENCIL_BUFFER_BIT ) )
		{
			result += sep + "GL_STENCIL_BUFFER_BIT";
		}

		return result;
	}

	GlImageAspectFlags convertImageAspectFlags( VkImageAspectFlags flags )
	{
		GLenum result{ 0 };

		if ( ashes::checkFlag( flags, VK_IMAGE_ASPECT_COLOR_BIT ) )
		{
			result |= GL_COLOR_BUFFER_BIT;
		}

		if ( ashes::checkFlag( flags, VK_IMAGE_ASPECT_DEPTH_BIT ) )
		{
			result |= GL_DEPTH_BUFFER_BIT;
		}

		if ( ashes::checkFlag( flags, VK_IMAGE_ASPECT_STENCIL_BIT ) )
		{
			result |= GL_STENCIL_BUFFER_BIT;
		}

		if ( ashes::checkFlag( flags, VK_IMAGE_ASPECT_METADATA_BIT ) )
		{
		}

		return result;
	}
}
