#include "D3D11RendererPrerequisites.hpp"

namespace d3d11_renderer
{
	D3D11_BLEND_OP convert( renderer::BlendOp const & value )
	{
		switch ( value )
		{
		case renderer::BlendOp::eAdd:
			return D3D11_BLEND_OP_ADD;

		case renderer::BlendOp::eSubtract:
			return D3D11_BLEND_OP_SUBTRACT;

		case renderer::BlendOp::eReverseSubtract:
			return D3D11_BLEND_OP_REV_SUBTRACT;

		case renderer::BlendOp::eMin:
			return D3D11_BLEND_OP_MIN;

		case renderer::BlendOp::eMax:
			return D3D11_BLEND_OP_MAX;

		default:
			assert( false && "Unsupported blend operator" );
			return D3D11_BLEND_OP_ADD;
		}
	}
}
