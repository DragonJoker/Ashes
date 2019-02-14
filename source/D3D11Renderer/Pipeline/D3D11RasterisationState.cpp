/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "D3D11RendererPrerequisites.hpp"

#include <Ashes/Pipeline/MultisampleState.hpp>

namespace d3d11_renderer
{
	D3D11_RASTERIZER_DESC convert( ashes::RasterisationState const & state
		, ashes::MultisampleState const & ms )
	{
		return D3D11_RASTERIZER_DESC
		{
			convert( state.polygonMode ),
			convert( state.cullMode ),
			state.frontFace == ashes::FrontFace::eCounterClockwise,
			INT( state.depthBiasConstantFactor ),
			state.depthBiasClamp,
			state.depthBiasSlopeFactor,
			state.depthClampEnable,
			FALSE,
			ms.rasterisationSamples != ashes::SampleCountFlag::e1,
			FALSE,
		};
	}
}
