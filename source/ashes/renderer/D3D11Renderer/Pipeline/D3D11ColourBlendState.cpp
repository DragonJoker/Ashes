/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "D3D11RendererPrerequisites.hpp"

namespace ashes::d3d11
{
	D3D11_BLEND_DESC convert( VkPipelineColorBlendStateCreateInfo const & state )
	{
		D3D11_BLEND_DESC result{};
		result.IndependentBlendEnable = true;
		uint32_t i = 0u;

		for ( auto & attach : ashes::makeArrayView( state.pAttachments, state.attachmentCount ) )
		{
			result.RenderTarget[i].BlendEnable = attach.blendEnable;
			result.RenderTarget[i].BlendOp = getBlendOp( attach.colorBlendOp );
			result.RenderTarget[i].BlendOpAlpha = getBlendOp( attach.alphaBlendOp );
			result.RenderTarget[i].DestBlend = getBlendFactor( attach.dstColorBlendFactor );
			result.RenderTarget[i].DestBlendAlpha = getBlendFactor( attach.dstAlphaBlendFactor );
			result.RenderTarget[i].RenderTargetWriteMask = UINT8( attach.colorWriteMask );
			result.RenderTarget[i].SrcBlend = getBlendFactor( attach.srcColorBlendFactor );
			result.RenderTarget[i].SrcBlendAlpha = getBlendFactor( attach.srcAlphaBlendFactor );
			++i;
		}

		return result;
	}
}
