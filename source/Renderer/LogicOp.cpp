#include "LogicOp.hpp"

namespace renderer
{
	VkLogicOp convert( LogicOp const & value )
	{
		switch ( value )
		{
		case LogicOp::eClear:
			return VK_LOGIC_OP_CLEAR;

		case LogicOp::eAnd:
			return VK_LOGIC_OP_AND;

		case LogicOp::eAndReverse:
			return VK_LOGIC_OP_AND_REVERSE;

		case LogicOp::eCopy:
			return VK_LOGIC_OP_COPY;

		case LogicOp::eAndInverted:
			return VK_LOGIC_OP_AND_INVERTED;

		case LogicOp::eNoOp:
			return VK_LOGIC_OP_NO_OP;

		case LogicOp::eXor:
			return VK_LOGIC_OP_XOR;

		case LogicOp::eOr:
			return VK_LOGIC_OP_OR;

		case LogicOp::eNor:
			return VK_LOGIC_OP_NOR;

		case LogicOp::eEquivalent:
			return VK_LOGIC_OP_EQUIVALENT;

		case LogicOp::eInvert:
			return VK_LOGIC_OP_INVERT;

		case LogicOp::eOrReverse:
			return VK_LOGIC_OP_OR_REVERSE;

		case LogicOp::eCopyInverted:
			return VK_LOGIC_OP_COPY_INVERTED;

		case LogicOp::eOrInverted:
			return VK_LOGIC_OP_OR_INVERTED;

		case LogicOp::eNand:
			return VK_LOGIC_OP_NAND;

		case LogicOp::eSet:
			return VK_LOGIC_OP_SET;

		default:
			assert( false && "Unsupported logic operator");
			return VK_LOGIC_OP_COPY;
		}
	}
}
