#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkLogicOp convert( ashes::LogicOp const & value )
	{
		switch ( value )
		{
		case ashes::LogicOp::eClear:
			return VK_LOGIC_OP_CLEAR;

		case ashes::LogicOp::eAnd:
			return VK_LOGIC_OP_AND;

		case ashes::LogicOp::eAndReverse:
			return VK_LOGIC_OP_AND_REVERSE;

		case ashes::LogicOp::eCopy:
			return VK_LOGIC_OP_COPY;

		case ashes::LogicOp::eAndInverted:
			return VK_LOGIC_OP_AND_INVERTED;

		case ashes::LogicOp::eNoOp:
			return VK_LOGIC_OP_NO_OP;

		case ashes::LogicOp::eXor:
			return VK_LOGIC_OP_XOR;

		case ashes::LogicOp::eOr:
			return VK_LOGIC_OP_OR;

		case ashes::LogicOp::eNor:
			return VK_LOGIC_OP_NOR;

		case ashes::LogicOp::eEquivalent:
			return VK_LOGIC_OP_EQUIVALENT;

		case ashes::LogicOp::eInvert:
			return VK_LOGIC_OP_INVERT;

		case ashes::LogicOp::eOrReverse:
			return VK_LOGIC_OP_OR_REVERSE;

		case ashes::LogicOp::eCopyInverted:
			return VK_LOGIC_OP_COPY_INVERTED;

		case ashes::LogicOp::eOrInverted:
			return VK_LOGIC_OP_OR_INVERTED;

		case ashes::LogicOp::eNand:
			return VK_LOGIC_OP_NAND;

		case ashes::LogicOp::eSet:
			return VK_LOGIC_OP_SET;

		default:
			assert( false && "Unsupported logic operator");
			return VK_LOGIC_OP_COPY;
		}
	}
}
