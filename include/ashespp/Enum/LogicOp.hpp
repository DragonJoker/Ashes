/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_LogicOp_HPP___
#define ___AshesPP_LogicOp_HPP___
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
	inline std::string getName( VkLogicOp value )
	{
		switch ( value )
		{
		case VK_LOGIC_OP_CLEAR:
			return "clear";
		case VK_LOGIC_OP_AND:
			return "and";
		case VK_LOGIC_OP_AND_REVERSE:
			return "and_reverse";
		case VK_LOGIC_OP_COPY:
			return "copy";
		case VK_LOGIC_OP_AND_INVERTED:
			return "and_inverted";
		case VK_LOGIC_OP_NO_OP:
			return "noop";
		case VK_LOGIC_OP_XOR:
			return "xor";
		case VK_LOGIC_OP_OR:
			return "or";
		case VK_LOGIC_OP_NOR:
			return "nor";
		case VK_LOGIC_OP_EQUIVALENT:
			return "equivalent";
		case VK_LOGIC_OP_INVERT:
			return "invert";
		case VK_LOGIC_OP_OR_REVERSE:
			return "or_reverse";
		case VK_LOGIC_OP_COPY_INVERTED:
			return "copy_inverted";
		case VK_LOGIC_OP_OR_INVERTED:
			return "or_inverted";
		case VK_LOGIC_OP_NAND:
			return "nand";
		case VK_LOGIC_OP_SET:
			return "set";
		default:
			assert( false && "Unsupported VkLogicOp." );
			throw std::runtime_error{ "Unsupported VkLogicOp" };
		}

		return 0;
	}
}

#endif
