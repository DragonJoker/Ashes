/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_QueryResultFlags_HPP___
#define ___AshesPP_QueryResultFlags_HPP___
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
	inline std::string getName( VkQueryResultFlagBits value )
	{
		switch ( value )
		{
		case VK_QUERY_RESULT_64_BIT:
			return "64";
		case VK_QUERY_RESULT_WAIT_BIT:
			return "wait";
		case VK_QUERY_RESULT_WITH_AVAILABILITY_BIT:
			return "with_availability";
		case VK_QUERY_RESULT_PARTIAL_BIT:
			return "partial";
		default:
			assert( false && "Unsupported VkQueryResultFlagBits." );
			return "Unsupported VkQueryResultFlagBits";
		}
	}
}

#endif
