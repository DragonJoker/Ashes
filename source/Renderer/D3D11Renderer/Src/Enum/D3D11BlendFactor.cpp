#include "D3D11RendererPrerequisites.hpp"

namespace d3d11_renderer
{
	D3D11_BLEND convert( renderer::BlendFactor const & value )
	{
		switch ( value )
		{
		case renderer::BlendFactor::eZero:
			return D3D11_BLEND_ZERO;

		case renderer::BlendFactor::eOne:
			return D3D11_BLEND_ONE;

		case renderer::BlendFactor::eSrcColour:
			return D3D11_BLEND_SRC_COLOR;

		case renderer::BlendFactor::eInvSrcColour:
			return D3D11_BLEND_INV_SRC_COLOR;

		case renderer::BlendFactor::eDstColour:
			return D3D11_BLEND_DEST_COLOR;

		case renderer::BlendFactor::eInvDstColour:
			return D3D11_BLEND_INV_DEST_COLOR;

		case renderer::BlendFactor::eSrcAlpha:
			return D3D11_BLEND_SRC_ALPHA;

		case renderer::BlendFactor::eInvSrcAlpha:
			return D3D11_BLEND_INV_SRC_ALPHA;

		case renderer::BlendFactor::eDstAlpha:
			return D3D11_BLEND_DEST_ALPHA;

		case renderer::BlendFactor::eInvDstAlpha:
			return D3D11_BLEND_INV_DEST_ALPHA;

		case renderer::BlendFactor::eSrc1Colour:
			return D3D11_BLEND_SRC1_COLOR;

		case renderer::BlendFactor::eInvSrc1Colour:
			return D3D11_BLEND_INV_SRC1_COLOR;

		case renderer::BlendFactor::eSrc1Alpha:
			return D3D11_BLEND_SRC_ALPHA;

		case renderer::BlendFactor::eInvSrc1Alpha:
			return D3D11_BLEND_INV_SRC1_ALPHA;

		default:
			assert( false && "Unsupported blend factor" );
			return D3D11_BLEND_ONE;
		}
	}
}
