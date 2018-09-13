#include "D3D11RendererPrerequisites.hpp"

namespace d3d11_renderer
{
	D3D11_STENCIL_OP convert( ashes::StencilOp const & value )
	{
		switch ( value )
		{
		case ashes::StencilOp::eKeep:
			return D3D11_STENCIL_OP_KEEP;

		case ashes::StencilOp::eZero:
			return D3D11_STENCIL_OP_ZERO;

		case ashes::StencilOp::eReplace:
			return D3D11_STENCIL_OP_REPLACE;

		case ashes::StencilOp::eIncrementAndClamp:
			return D3D11_STENCIL_OP_INCR_SAT;

		case ashes::StencilOp::eDecrementAndClamp:
			return D3D11_STENCIL_OP_DECR_SAT;

		case ashes::StencilOp::eInvert:
			return D3D11_STENCIL_OP_INVERT;

		default:
			assert( false && "Unsupported stencil operator");
			return D3D11_STENCIL_OP_KEEP;
		}
	}
}
