#include "GlRendererPrerequisites.hpp"

namespace gl_renderer
{
	GLsizei convert( VkSampleCountFlagBits const & flags )
	{
		switch ( flags )
		{
		case VK_SAMPLE_COUNT_1:
			return 1;

		case VK_SAMPLE_COUNT_2:
			return 2;

		case VK_SAMPLE_COUNT_4:
			return 4;

		case VK_SAMPLE_COUNT_8:
			return 8;

		case VK_SAMPLE_COUNT_16:
			return 16;

		case VK_SAMPLE_COUNT_32:
			return 32;

		case VK_SAMPLE_COUNT_64:
			return 64;

		default:
			assert( false && "Unsupported sample count flag." );
			return 1;
		}
	}
}
