#include "GlRendererPrerequisites.hpp"

namespace ashes::gl4
{
	GLsizei convert( VkSampleCountFlagBits const & flags )
	{
		switch ( flags )
		{
		case VK_SAMPLE_COUNT_1_BIT:
			return 1;

		case VK_SAMPLE_COUNT_2_BIT:
			return 2;

		case VK_SAMPLE_COUNT_4_BIT:
			return 4;

		case VK_SAMPLE_COUNT_8_BIT:
			return 8;

		case VK_SAMPLE_COUNT_16_BIT:
			return 16;

		case VK_SAMPLE_COUNT_32_BIT:
			return 32;

		case VK_SAMPLE_COUNT_64_BIT:
			return 64;

		default:
			assert( false && "Unsupported sample count flag." );
			return 1;
		}
	}
}
