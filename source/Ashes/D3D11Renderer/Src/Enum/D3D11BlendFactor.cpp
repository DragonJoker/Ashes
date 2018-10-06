#include "D3D11RendererPrerequisites.hpp"

namespace d3d11_renderer
{
	D3D11_BLEND convert( ashes::BlendFactor const & value )
	{
		switch ( value )
		{
		case ashes::BlendFactor::eZero:
			return D3D11_BLEND_ZERO;

		case ashes::BlendFactor::eOne:
			return D3D11_BLEND_ONE;

		case ashes::BlendFactor::eSrcColour:
			return D3D11_BLEND_SRC_COLOR;

		case ashes::BlendFactor::eInvSrcColour:
			return D3D11_BLEND_INV_SRC_COLOR;

		case ashes::BlendFactor::eDstColour:
			return D3D11_BLEND_DEST_COLOR;

		case ashes::BlendFactor::eInvDstColour:
			return D3D11_BLEND_INV_DEST_COLOR;

		case ashes::BlendFactor::eSrcAlpha:
			return D3D11_BLEND_SRC_ALPHA;

		case ashes::BlendFactor::eInvSrcAlpha:
			return D3D11_BLEND_INV_SRC_ALPHA;

		case ashes::BlendFactor::eDstAlpha:
			return D3D11_BLEND_DEST_ALPHA;

		case ashes::BlendFactor::eInvDstAlpha:
			return D3D11_BLEND_INV_DEST_ALPHA;

		case ashes::BlendFactor::eSrc1Colour:
			return D3D11_BLEND_SRC1_COLOR;

		case ashes::BlendFactor::eInvSrc1Colour:
			return D3D11_BLEND_INV_SRC1_COLOR;

		case ashes::BlendFactor::eSrc1Alpha:
			return D3D11_BLEND_SRC1_ALPHA;

		case ashes::BlendFactor::eInvSrc1Alpha:
			return D3D11_BLEND_INV_SRC1_ALPHA;

		default:
			assert( false && "Unsupported blend factor" );
			return D3D11_BLEND_ONE;
		}
	}
}
