/**
*\file
*	VertexBuffer.h
*\author
*	Sylvain Doremus
*/
#pragma once

#include "D3D11RendererPrerequisites.hpp"

#include "Shader/D3D11ShaderDesc.hpp"

#include <Pipeline/VertexInputState.hpp>

namespace d3d11_renderer
{
	std::vector< D3D11_INPUT_ELEMENT_DESC > convert( ashes::VertexInputState const & state
		, InputLayout const & inputLayout );
}
