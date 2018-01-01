#include "GlBlendFactor.hpp"

namespace gl_renderer
{
	GLenum convert( renderer::BlendFactor const & value )
	{
		switch ( value )
		{
		case renderer::BlendFactor::eZero:
			return GL_ZERO;

		case renderer::BlendFactor::eOne:
			return GL_ONE;

		case renderer::BlendFactor::eSrcColour:
			return GL_SRC_COLOR;

		case renderer::BlendFactor::eInvSrcColour:
			return GL_ONE_MINUS_SRC_COLOR;

		case renderer::BlendFactor::eDstColour:
			return GL_DST_COLOR;

		case renderer::BlendFactor::eInvDstColour:
			return GL_ONE_MINUS_DST_COLOR;

		case renderer::BlendFactor::eSrcAlpha:
			return GL_SRC_ALPHA;

		case renderer::BlendFactor::eInvSrcAlpha:
			return GL_ONE_MINUS_SRC_ALPHA;

		case renderer::BlendFactor::eDstAlpha:
			return GL_DST_ALPHA;

		case renderer::BlendFactor::eInvDstAlpha:
			return GL_ONE_MINUS_DST_ALPHA;

		case renderer::BlendFactor::eConstantColour:
			return GL_CONSTANT_COLOR;

		case renderer::BlendFactor::eInvConstantColour:
			return GL_ONE_MINUS_CONSTANT_COLOR;

		case renderer::BlendFactor::eConstantAlpha:
			return GL_CONSTANT_ALPHA;

		case renderer::BlendFactor::eInvConstantAlpha:
			return GL_ONE_MINUS_CONSTANT_ALPHA;

		case renderer::BlendFactor::eSrc1Colour:
			return GL_SRC1_COLOR;

		case renderer::BlendFactor::eInvSrc1Colour:
			return GL_ONE_MINUS_SRC1_COLOR;

		case renderer::BlendFactor::eSrc1Alpha:
			return GL_SRC1_ALPHA;

		case renderer::BlendFactor::eInvSrc1Alpha:
			return GL_ONE_MINUS_SRC1_ALPHA;

		default:
			assert( false && "Unsupported blend factor" );
			return GL_ONE;
		}
	}
}
