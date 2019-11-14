/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_Filter_HPP___
#define ___AshesPP_Filter_HPP___
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
	inline std::string getName( VkFilter value )
	{
		switch ( value )
		{
		case VK_FILTER_NEAREST:
			return "nearest";
		case VK_FILTER_LINEAR:
			return "linear";
		case VK_FILTER_CUBIC_IMG:
			return "cubic";
		default:
			assert( false && "Unsupported VkFilter." );
			throw std::runtime_error{ "Unsupported VkFilter" };
		}

		return 0;
	}
}

#endif
