#include "SampleCountFlag.hpp"

namespace renderer
{
	VkSampleCountFlagBits convert( SampleCountFlag const & flags )
	{
		switch ( flags )
		{
		case SampleCountFlag::e1:
			return VK_SAMPLE_COUNT_1_BIT;

		case SampleCountFlag::e2:
			return VK_SAMPLE_COUNT_2_BIT;

		case SampleCountFlag::e4:
			return VK_SAMPLE_COUNT_4_BIT;

		case SampleCountFlag::e8:
			return VK_SAMPLE_COUNT_8_BIT;

		case SampleCountFlag::e16:
			return VK_SAMPLE_COUNT_16_BIT;

		case SampleCountFlag::e32:
			return VK_SAMPLE_COUNT_32_BIT;

		case SampleCountFlag::e64:
			return VK_SAMPLE_COUNT_64_BIT;

		default:
			assert( false && "Unsupported sample count flag." );
			return VK_SAMPLE_COUNT_1_BIT;
		}
	}
}
