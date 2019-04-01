#include "GlRendererPrerequisites.hpp"

namespace gl_renderer
{
	std::string getName( GlImageAspectFlags value )
	{
		std::string result;
		std::string sep;

		if ( checkFlag( value, GlImageAspectFlag::GL_COLOR_BUFFER_BIT ) )
		{
			result += sep + "GL_COLOR_BUFFER_BIT";
			sep = " | ";
		}

		if ( checkFlag( value, GlImageAspectFlag::GL_DEPTH_BUFFER_BIT ) )
		{
			result += sep + "GL_DEPTH_BUFFER_BIT";
			sep = " | ";
		}

		if ( checkFlag( value, GlImageAspectFlag::GL_STENCIL_BUFFER_BIT ) )
		{
			result += sep + "GL_STENCIL_BUFFER_BIT";
		}

		return result;
	}

	GlImageAspectFlags convert( VkImageAspectFlags const & flags )
	{
		GLenum result{ 0 };

		if ( checkFlag( flags, VkImageAspectFlagBits::eColour ) )
		{
			result |= GL_COLOR_BUFFER_BIT;
		}

		if ( checkFlag( flags, VkImageAspectFlagBits::eDepth ) )
		{
			result |= GL_DEPTH_BUFFER_BIT;
		}

		if ( checkFlag( flags, VkImageAspectFlagBits::eStencil ) )
		{
			result |= GL_STENCIL_BUFFER_BIT;
		}

		if ( checkFlag( flags, VkImageAspectFlagBits::eMetaData ) )
		{
		}

		return result;
	}
}
