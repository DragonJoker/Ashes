/**
*\file
*	VertexBuffer.h
*\author
*	Sylvain Doremus
*/
#pragma once

#include "D3D11Renderer/D3D11RendererPrerequisites.hpp"
#include "D3D11Renderer/Shader/D3D11ShaderDesc.hpp"

#include <Ashes/Pipeline/VertexInputState.hpp>

namespace ashes::d3d11
{
	std::vector< D3D11_INPUT_ELEMENT_DESC > convert( ashes::VertexInputState const & state
		, InputLayout const & inputLayout );
}
