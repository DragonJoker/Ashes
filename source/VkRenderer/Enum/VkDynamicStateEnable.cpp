#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkDynamicStateEnable convert( ashes::DynamicStateEnable const & value )
	{
		switch ( value )
		{
		case ashes::DynamicStateEnable::eViewport:
			return VK_DYNAMIC_STATE_VIEWPORT;
		case ashes::DynamicStateEnable::eScissor:
			return VK_DYNAMIC_STATE_SCISSOR;
		case ashes::DynamicStateEnable::eLineWidth:
			return VK_DYNAMIC_STATE_LINE_WIDTH;
		case ashes::DynamicStateEnable::eDepthBias:
			return VK_DYNAMIC_STATE_DEPTH_BIAS;
		case ashes::DynamicStateEnable::eBlendConstants:
			return VK_DYNAMIC_STATE_BLEND_CONSTANTS;
		case ashes::DynamicStateEnable::eDepthBounds:
			return VK_DYNAMIC_STATE_DEPTH_BOUNDS;
		case ashes::DynamicStateEnable::eStencilCompareMask:
			return VK_DYNAMIC_STATE_STENCIL_COMPARE_MASK;
		case ashes::DynamicStateEnable::eStencilWriteMask:
			return VK_DYNAMIC_STATE_STENCIL_WRITE_MASK;
		case ashes::DynamicStateEnable::eStencilReference:
			return VK_DYNAMIC_STATE_STENCIL_REFERENCE;
		default:
			assert( false && "Unsupported DynamicStateEnable" );
			return VK_DYNAMIC_STATE_VIEWPORT;
			break;
		}
	}
}
