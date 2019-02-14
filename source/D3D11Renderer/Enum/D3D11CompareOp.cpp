#include "D3D11RendererPrerequisites.hpp"

namespace d3d11_renderer
{
	D3D11_COMPARISON_FUNC convert( ashes::CompareOp const & value )
	{
		switch ( value )
		{
		case ashes::CompareOp::eNever:
			return D3D11_COMPARISON_NEVER;

		case ashes::CompareOp::eLess:
			return D3D11_COMPARISON_LESS;

		case ashes::CompareOp::eEqual:
			return D3D11_COMPARISON_EQUAL;

		case ashes::CompareOp::eLessEqual:
			return D3D11_COMPARISON_LESS_EQUAL;

		case ashes::CompareOp::eGreater:
			return D3D11_COMPARISON_GREATER;

		case ashes::CompareOp::eNotEqual:
			return D3D11_COMPARISON_NOT_EQUAL;

		case ashes::CompareOp::eGreaterEqual:
			return D3D11_COMPARISON_GREATER_EQUAL;

		case ashes::CompareOp::eAlways:
			return D3D11_COMPARISON_ALWAYS;

		default:
			assert( false && "Unsupported comparison operator" );
			return D3D11_COMPARISON_ALWAYS;
		}
	}
}
