/**
*\file
*	VertexBuffer.h
*\author
*	Sylvain Doremus
*/
#pragma once

#include "renderer/XBoxRenderer/XBoxRendererPrerequisites.hpp"
#include "renderer/XBoxRenderer/Shader/XBoxShaderDesc.hpp"

namespace ashes::xbox
{
	std::vector< D3D11_INPUT_ELEMENT_DESC > convert( VkPipelineVertexInputStateCreateInfo const & state
		, InputLayout const & inputLayout );
}
