#include "CompareOp.hpp"

namespace renderer
{
	VkCompareOp convert( CompareOp const & value )
	{
		switch ( value )
		{
		case CompareOp::eNever:
			return VK_COMPARE_OP_NEVER;

		case CompareOp::eLess:
			return VK_COMPARE_OP_LESS;

		case CompareOp::eEqual:
			return VK_COMPARE_OP_EQUAL;

		case CompareOp::eLessEqual:
			return VK_COMPARE_OP_LESS_OR_EQUAL;

		case CompareOp::eGreater:
			return VK_COMPARE_OP_GREATER;

		case CompareOp::eNotEqual:
			return VK_COMPARE_OP_NOT_EQUAL;

		case CompareOp::eGreaterEqual:
			return VK_COMPARE_OP_GREATER_OR_EQUAL;

		case CompareOp::eAlways:
			return VK_COMPARE_OP_ALWAYS;

		default:
			assert( false && "Unsupported comparison operator" );
			return VK_COMPARE_OP_ALWAYS;
		}
	}
}
