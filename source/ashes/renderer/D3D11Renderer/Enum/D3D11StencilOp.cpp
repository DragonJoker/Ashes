#include "D3D11RendererPrerequisites.hpp"

namespace ashes::d3d11
{
	D3D11_STENCIL_OP getStencilOp( VkStencilOp const & value )
	{
		switch ( value )
		{
		case VK_STENCIL_OP_KEEP:
			return D3D11_STENCIL_OP_KEEP;

		case VK_STENCIL_OP_ZERO:
			return D3D11_STENCIL_OP_ZERO;

		case VK_STENCIL_OP_REPLACE:
			return D3D11_STENCIL_OP_REPLACE;

		case VK_STENCIL_OP_INCREMENT_AND_WRAP:
			return D3D11_STENCIL_OP_INCR;

		case VK_STENCIL_OP_DECREMENT_AND_WRAP:
			return D3D11_STENCIL_OP_DECR;

		case VK_STENCIL_OP_INCREMENT_AND_CLAMP:
			return D3D11_STENCIL_OP_INCR_SAT;

		case VK_STENCIL_OP_DECREMENT_AND_CLAMP:
			return D3D11_STENCIL_OP_DECR_SAT;

		case VK_STENCIL_OP_INVERT:
			return D3D11_STENCIL_OP_INVERT;

		default:
			assert( false && "Unsupported stencil operator");
			return D3D11_STENCIL_OP_KEEP;
		}
	}
}
