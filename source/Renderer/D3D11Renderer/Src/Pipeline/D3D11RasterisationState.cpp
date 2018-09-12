/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#include "D3D11RendererPrerequisites.hpp"

#include <Pipeline/MultisampleState.hpp>

namespace d3d11_renderer
{
	D3D11_RASTERIZER_DESC convert( renderer::RasterisationState const & state
		, renderer::MultisampleState const & ms )
	{
		return D3D11_RASTERIZER_DESC
		{
			convert( state.polygonMode ),
			convert( state.cullMode ),
			state.frontFace == renderer::FrontFace::eCounterClockwise,
			INT( state.depthBiasConstantFactor ),
			state.depthBiasClamp,
			state.depthBiasSlopeFactor,
			state.depthClampEnable,
			FALSE,
			ms.rasterisationSamples != renderer::SampleCountFlag::e1,
			FALSE,
		};
	}
}
