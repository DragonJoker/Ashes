/**
*\file
*	VertexBuffer.h
*\author
*	Sylvain Doremus
*/
#pragma once

#include "renderer/D3D11Renderer/D3D11RendererPrerequisites.hpp"
#include "renderer/D3D11Renderer/Shader/D3D11ShaderDesc.hpp"

namespace ashes::D3D11_NAMESPACE
{
	std::vector< D3D11_INPUT_ELEMENT_DESC > convert( VkPipelineVertexInputStateCreateInfo const & state
		, InputLayout const & inputLayout );
}
