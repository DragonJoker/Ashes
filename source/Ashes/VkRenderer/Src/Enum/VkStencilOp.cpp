#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkStencilOp convert( ashes::StencilOp const & value )
	{
		switch ( value )
		{
		case ashes::StencilOp::eKeep:
			return VK_STENCIL_OP_KEEP;

		case ashes::StencilOp::eZero:
			return VK_STENCIL_OP_ZERO;

		case ashes::StencilOp::eReplace:
			return VK_STENCIL_OP_REPLACE;

		case ashes::StencilOp::eIncrementAndClamp:
			return VK_STENCIL_OP_INCREMENT_AND_CLAMP;

		case ashes::StencilOp::eDecrementAndClamp:
			return VK_STENCIL_OP_DECREMENT_AND_CLAMP;

		case ashes::StencilOp::eInvert:
			return VK_STENCIL_OP_INVERT;

		case ashes::StencilOp::eIncrementAndWrap:
			return VK_STENCIL_OP_INCREMENT_AND_WRAP;

		case ashes::StencilOp::eDecrementAndWrap:
			return VK_STENCIL_OP_DECREMENT_AND_WRAP;

		default:
			assert( false && "Unsupported stencil operator");
			return VK_STENCIL_OP_KEEP;
		}
	}
}
