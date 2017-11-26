#include "BlendFactor.hpp"

namespace renderer
{
	VkBlendFactor convert( BlendFactor const & value )
	{
		switch ( value )
		{
		case BlendFactor::eZero:
			return VK_BLEND_FACTOR_ZERO;

		case BlendFactor::eOne:
			return VK_BLEND_FACTOR_ONE;

		case BlendFactor::eSrcColour:
			return VK_BLEND_FACTOR_SRC_COLOR;

		case BlendFactor::eInvSrcColour:
			return VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;

		case BlendFactor::eDstColour:
			return VK_BLEND_FACTOR_DST_COLOR;

		case BlendFactor::eInvDstColour:
			return VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;

		case BlendFactor::eSrcAlpha:
			return VK_BLEND_FACTOR_SRC_ALPHA;

		case BlendFactor::eInvSrcAlpha:
			return VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;

		case BlendFactor::eDstAlpha:
			return VK_BLEND_FACTOR_DST_ALPHA;

		case BlendFactor::eInvDstAlpha:
			return VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;

		case BlendFactor::eConstantColour:
			return VK_BLEND_FACTOR_CONSTANT_COLOR;

		case BlendFactor::eInvConstantColour:
			return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR;

		case BlendFactor::eConstantAlpha:
			return VK_BLEND_FACTOR_CONSTANT_ALPHA;

		case BlendFactor::eInvConstantAlpha:
			return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA;

		case BlendFactor::eSrc1Colour:
			return VK_BLEND_FACTOR_SRC1_COLOR;

		case BlendFactor::eInvSrc1Colour:
			return VK_BLEND_FACTOR_ONE_MINUS_SRC1_COLOR;

		case BlendFactor::eSrc1Alpha:
			return VK_BLEND_FACTOR_SRC_ALPHA;

		case BlendFactor::eInvSrc1Alpha:
			return VK_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA;

		default:
			assert( false && "Unsupported blend factor" );
			return VK_BLEND_FACTOR_ONE;
		}
	}
}
