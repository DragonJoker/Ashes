#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkDynamicState convert( ashes::DynamicState const & value )
	{
		switch ( value )
		{
		case ashes::DynamicState::eViewport:
			return VK_DYNAMIC_STATE_VIEWPORT;
		case ashes::DynamicState::eScissor:
			return VK_DYNAMIC_STATE_SCISSOR;
		case ashes::DynamicState::eLineWidth:
			return VK_DYNAMIC_STATE_LINE_WIDTH;
		case ashes::DynamicState::eDepthBias:
			return VK_DYNAMIC_STATE_DEPTH_BIAS;
		case ashes::DynamicState::eBlendConstants:
			return VK_DYNAMIC_STATE_BLEND_CONSTANTS;
		case ashes::DynamicState::eDepthBounds:
			return VK_DYNAMIC_STATE_DEPTH_BOUNDS;
		case ashes::DynamicState::eStencilCompareMask:
			return VK_DYNAMIC_STATE_STENCIL_COMPARE_MASK;
		case ashes::DynamicState::eStencilWriteMask:
			return VK_DYNAMIC_STATE_STENCIL_WRITE_MASK;
		case ashes::DynamicState::eStencilReference:
			return VK_DYNAMIC_STATE_STENCIL_REFERENCE;
		default:
			assert( false && "Unsupported DynamicState" );
			return VK_DYNAMIC_STATE_VIEWPORT;
			break;
		}
	}
}
