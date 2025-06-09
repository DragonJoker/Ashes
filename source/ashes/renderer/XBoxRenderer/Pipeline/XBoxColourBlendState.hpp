/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once
#include <d3d11.h>


namespace ashes::xbox
{
	D3D11_BLEND_DESC convert( VkPipelineColorBlendStateCreateInfo const & state );
}
