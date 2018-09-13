/**
*\file
*	VertexBuffer.h
*\author
*	Sylvain Doremus
*/
#pragma once

#include <Pipeline/VertexInputState.hpp>

namespace d3d11_renderer
{
	std::vector< D3D11_INPUT_ELEMENT_DESC > convert( renderer::VertexInputState const & state
		, std::list< std::string > & strings );
}
