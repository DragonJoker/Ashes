#include "XBoxRendererPrerequisites.hpp"

namespace ashes::xbox
{
	D3D11_COMPARISON_FUNC getComparisonFunc( VkCompareOp const & value )
	{
		switch ( value )
		{
		case VK_COMPARE_OP_NEVER:
			return D3D11_COMPARISON_NEVER;

		case VK_COMPARE_OP_LESS:
			return D3D11_COMPARISON_LESS;

		case VK_COMPARE_OP_EQUAL:
			return D3D11_COMPARISON_EQUAL;

		case VK_COMPARE_OP_LESS_OR_EQUAL:
			return D3D11_COMPARISON_LESS_EQUAL;

		case VK_COMPARE_OP_GREATER:
			return D3D11_COMPARISON_GREATER;

		case VK_COMPARE_OP_NOT_EQUAL:
			return D3D11_COMPARISON_NOT_EQUAL;

		case VK_COMPARE_OP_GREATER_OR_EQUAL:
			return D3D11_COMPARISON_GREATER_EQUAL;

		case VK_COMPARE_OP_ALWAYS:
			return D3D11_COMPARISON_ALWAYS;

		default:
			assert( false && "Unsupported comparison operator" );
			return D3D11_COMPARISON_ALWAYS;
		}
	}
}
