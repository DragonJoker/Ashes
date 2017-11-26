#include "BlendOp.hpp"

namespace renderer
{
	VkBlendOp convert( BlendOp const & value )
	{
		switch ( value )
		{
		case BlendOp::eAdd:
			return VK_BLEND_OP_ADD;

		case BlendOp::eSubtract:
			return VK_BLEND_OP_SUBTRACT;

		case BlendOp::eReverseSubtract:
			return VK_BLEND_OP_REVERSE_SUBTRACT;

		case BlendOp::eMin:
			return VK_BLEND_OP_MIN;

		case BlendOp::eMax:
			return VK_BLEND_OP_MAX;

		default:
			assert( false && "Unsupported blend operator" );
			return VK_BLEND_OP_ADD;
		}
	}
}
