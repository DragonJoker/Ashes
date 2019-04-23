/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "D3D11RendererPrerequisites.hpp"

namespace ashes::d3d11
{
	D3D11_RASTERIZER_DESC convert( VkPipelineRasterizationStateCreateInfo const & state
		, VkPipelineMultisampleStateCreateInfo const & ms )
	{
		return D3D11_RASTERIZER_DESC
		{
			getFillMode( state.polygonMode ),
			getCullMode( state.cullMode ),
			state.frontFace == VK_FRONT_FACE_COUNTER_CLOCKWISE,
			INT( state.depthBiasConstantFactor ),
			state.depthBiasClamp,
			state.depthBiasSlopeFactor,
			state.depthClampEnable,
			FALSE,
			ms.rasterizationSamples != VK_SAMPLE_COUNT_1_BIT,
			FALSE,
		};
	}
}
