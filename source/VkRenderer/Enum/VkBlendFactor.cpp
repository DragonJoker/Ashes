#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkBlendFactor convert( ashes::BlendFactor const & value )
	{
		switch ( value )
		{
		case ashes::BlendFactor::eZero:
			return VK_BLEND_FACTOR_ZERO;

		case ashes::BlendFactor::eOne:
			return VK_BLEND_FACTOR_ONE;

		case ashes::BlendFactor::eSrcColour:
			return VK_BLEND_FACTOR_SRC_COLOR;

		case ashes::BlendFactor::eInvSrcColour:
			return VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;

		case ashes::BlendFactor::eDstColour:
			return VK_BLEND_FACTOR_DST_COLOR;

		case ashes::BlendFactor::eInvDstColour:
			return VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;

		case ashes::BlendFactor::eSrcAlpha:
			return VK_BLEND_FACTOR_SRC_ALPHA;

		case ashes::BlendFactor::eInvSrcAlpha:
			return VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;

		case ashes::BlendFactor::eDstAlpha:
			return VK_BLEND_FACTOR_DST_ALPHA;

		case ashes::BlendFactor::eInvDstAlpha:
			return VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;

		case ashes::BlendFactor::eConstantColour:
			return VK_BLEND_FACTOR_CONSTANT_COLOR;

		case ashes::BlendFactor::eInvConstantColour:
			return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR;

		case ashes::BlendFactor::eConstantAlpha:
			return VK_BLEND_FACTOR_CONSTANT_ALPHA;

		case ashes::BlendFactor::eInvConstantAlpha:
			return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA;

		case ashes::BlendFactor::eSrc1Colour:
			return VK_BLEND_FACTOR_SRC1_COLOR;

		case ashes::BlendFactor::eInvSrc1Colour:
			return VK_BLEND_FACTOR_ONE_MINUS_SRC1_COLOR;

		case ashes::BlendFactor::eSrc1Alpha:
			return VK_BLEND_FACTOR_SRC_ALPHA;

		case ashes::BlendFactor::eInvSrc1Alpha:
			return VK_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA;

		default:
			assert( false && "Unsupported blend factor" );
			return VK_BLEND_FACTOR_ONE;
		}
	}
}
