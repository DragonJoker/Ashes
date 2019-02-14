#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkCompareOp convert( ashes::CompareOp const & value )
	{
		switch ( value )
		{
		case ashes::CompareOp::eNever:
			return VK_COMPARE_OP_NEVER;

		case ashes::CompareOp::eLess:
			return VK_COMPARE_OP_LESS;

		case ashes::CompareOp::eEqual:
			return VK_COMPARE_OP_EQUAL;

		case ashes::CompareOp::eLessEqual:
			return VK_COMPARE_OP_LESS_OR_EQUAL;

		case ashes::CompareOp::eGreater:
			return VK_COMPARE_OP_GREATER;

		case ashes::CompareOp::eNotEqual:
			return VK_COMPARE_OP_NOT_EQUAL;

		case ashes::CompareOp::eGreaterEqual:
			return VK_COMPARE_OP_GREATER_OR_EQUAL;

		case ashes::CompareOp::eAlways:
			return VK_COMPARE_OP_ALWAYS;

		default:
			assert( false && "Unsupported comparison operator" );
			return VK_COMPARE_OP_ALWAYS;
		}
	}
}
