/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_SampleCountFlags_HPP___
#define ___AshesPP_SampleCountFlags_HPP___
#pragma once

namespace ashes
{
	/**
	*\brief
	*	Gets the name of the given element type.
	*\param[in] value
	*	The element type.
	*\return
	*	The name.
	*/
	inline std::string getName( VkSampleCountFlagBits value )
	{
		switch ( value )
		{
		case VK_SAMPLE_COUNT_1_BIT:
			return "1x";
		case VK_SAMPLE_COUNT_2_BIT:
			return "2x";
		case VK_SAMPLE_COUNT_4_BIT:
			return "4x";
		case VK_SAMPLE_COUNT_8_BIT:
			return "8x";
		case VK_SAMPLE_COUNT_16_BIT:
			return "16x";
		case VK_SAMPLE_COUNT_32_BIT:
			return "32x";
		case VK_SAMPLE_COUNT_64_BIT:
			return "64x";
		default:
			assert( false && "Unsupported VkSampleCountFlagBits." );
			throw std::runtime_error{ "Unsupported VkSampleCountFlagBits" };
		}

		return 0;
	}
}

#endif
