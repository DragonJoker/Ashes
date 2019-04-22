/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "D3D11RendererPrerequisites.hpp"

namespace ashes::d3d11
{
	D3D11_BLEND_DESC convert( ashes::ColourBlendState const & state )
	{
		D3D11_BLEND_DESC result{};
		result.IndependentBlendEnable = true;
		uint32_t i = 0u;

		for ( auto & attach : state.attachs )
		{
			result.RenderTarget[i].BlendEnable = attach.blendEnable;
			result.RenderTarget[i].BlendOp = convert( attach.colorBlendOp );
			result.RenderTarget[i].BlendOpAlpha = convert( attach.alphaBlendOp );
			result.RenderTarget[i].DestBlend = convert( attach.dstColorBlendFactor );
			result.RenderTarget[i].DestBlendAlpha = convert( attach.dstAlphaBlendFactor );
			result.RenderTarget[i].RenderTargetWriteMask = UINT8( attach.colorWriteMask );
			result.RenderTarget[i].SrcBlend = convert( attach.srcColorBlendFactor );
			result.RenderTarget[i].SrcBlendAlpha = convert( attach.srcAlphaBlendFactor );
			++i;
		}

		return result;
	}
}
