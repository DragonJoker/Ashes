#include "StencilOp.hpp"

namespace renderer
{
	VkStencilOp convert( StencilOp const & value )
	{
		switch ( value )
		{
		case StencilOp::eKeep:
			return VK_STENCIL_OP_KEEP;

		case StencilOp::eZero:
			return VK_STENCIL_OP_ZERO;

		case StencilOp::eReplace:
			return VK_STENCIL_OP_REPLACE;

		case StencilOp::eIncrementAndClamp:
			return VK_STENCIL_OP_INCREMENT_AND_CLAMP;

		case StencilOp::eDecrementAndClamp:
			return VK_STENCIL_OP_DECREMENT_AND_CLAMP;

		case StencilOp::eInvert:
			return VK_STENCIL_OP_INVERT;

		case StencilOp::eIncrementAndWrap:
			return VK_STENCIL_OP_INCREMENT_AND_WRAP;

		case StencilOp::eDecrementAndWrap:
			return VK_STENCIL_OP_DECREMENT_AND_WRAP;

		default:
			assert( false && "Unsupported stencil operator");
			return VK_STENCIL_OP_KEEP;
		}
	}
}
