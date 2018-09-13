#include "D3D11RendererPrerequisites.hpp"

namespace d3d11_renderer
{
	D3D11_STENCIL_OP convert( renderer::StencilOp const & value )
	{
		switch ( value )
		{
		case renderer::StencilOp::eKeep:
			return D3D11_STENCIL_OP_KEEP;

		case renderer::StencilOp::eZero:
			return D3D11_STENCIL_OP_ZERO;

		case renderer::StencilOp::eReplace:
			return D3D11_STENCIL_OP_REPLACE;

		case renderer::StencilOp::eIncrementAndClamp:
			return D3D11_STENCIL_OP_INCR_SAT;

		case renderer::StencilOp::eDecrementAndClamp:
			return D3D11_STENCIL_OP_DECR_SAT;

		case renderer::StencilOp::eInvert:
			return D3D11_STENCIL_OP_INVERT;

		default:
			assert( false && "Unsupported stencil operator");
			return D3D11_STENCIL_OP_KEEP;
		}
	}
}
