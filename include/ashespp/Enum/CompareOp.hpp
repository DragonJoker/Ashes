/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_CompareOp_HPP___
#define ___AshesPP_CompareOp_HPP___
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
	inline std::string getName( VkCompareOp value )
	{
		switch ( value )
		{
		case VK_COMPARE_OP_NEVER:
			return "never";
		case VK_COMPARE_OP_LESS:
			return "less";
		case VK_COMPARE_OP_EQUAL:
			return "equal";
		case VK_COMPARE_OP_LESS_OR_EQUAL:
			return "less_equal";
		case VK_COMPARE_OP_GREATER:
			return "greater";
		case VK_COMPARE_OP_NOT_EQUAL:
			return "not_equal";
		case VK_COMPARE_OP_GREATER_OR_EQUAL:
			return "greater_equal";
		case VK_COMPARE_OP_ALWAYS:
			return "always";
		default:
			assert( false && "Unsupported VkCompareOp." );
			throw std::runtime_error{ "Unsupported VkCompareOp" };
		}

		return 0;
	}
}

#endif
