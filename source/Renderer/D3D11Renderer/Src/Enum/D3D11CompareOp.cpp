#include "D3D11RendererPrerequisites.hpp"

namespace d3d11_renderer
{
	D3D11_COMPARISON_FUNC convert( renderer::CompareOp const & value )
	{
		switch ( value )
		{
		case renderer::CompareOp::eNever:
			return D3D11_COMPARISON_NEVER;

		case renderer::CompareOp::eLess:
			return D3D11_COMPARISON_LESS;

		case renderer::CompareOp::eEqual:
			return D3D11_COMPARISON_EQUAL;

		case renderer::CompareOp::eLessEqual:
			return D3D11_COMPARISON_LESS_EQUAL;

		case renderer::CompareOp::eGreater:
			return D3D11_COMPARISON_GREATER;

		case renderer::CompareOp::eNotEqual:
			return D3D11_COMPARISON_NOT_EQUAL;

		case renderer::CompareOp::eGreaterEqual:
			return D3D11_COMPARISON_GREATER_EQUAL;

		case renderer::CompareOp::eAlways:
			return D3D11_COMPARISON_ALWAYS;

		default:
			assert( false && "Unsupported comparison operator" );
			return D3D11_COMPARISON_ALWAYS;
		}
	}
}
