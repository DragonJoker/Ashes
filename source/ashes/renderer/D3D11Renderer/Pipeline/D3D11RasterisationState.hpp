/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

namespace ashes::d3d11
{
	D3D11_RASTERIZER_DESC convert( VkPipelineRasterizationStateCreateInfo const & state
		, VkPipelineMultisampleStateCreateInfo const & ms );
}
