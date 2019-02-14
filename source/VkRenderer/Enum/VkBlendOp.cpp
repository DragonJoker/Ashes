#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkBlendOp convert( ashes::BlendOp const & value )
	{
		switch ( value )
		{
		case ashes::BlendOp::eAdd:
			return VK_BLEND_OP_ADD;

		case ashes::BlendOp::eSubtract:
			return VK_BLEND_OP_SUBTRACT;

		case ashes::BlendOp::eReverseSubtract:
			return VK_BLEND_OP_REVERSE_SUBTRACT;

		case ashes::BlendOp::eMin:
			return VK_BLEND_OP_MIN;

		case ashes::BlendOp::eMax:
			return VK_BLEND_OP_MAX;

		default:
			assert( false && "Unsupported blend operator" );
			return VK_BLEND_OP_ADD;
		}
	}
}
