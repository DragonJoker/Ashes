#include "Core/D3D11CreateRenderer.hpp"
#include "Core/D3D11Renderer.hpp"

extern "C"
{
	D3D11Renderer_API ashes::Renderer * createRenderer( ashes::Renderer::Configuration const & configuration )
	{
		return new d3d11_renderer::Renderer{ configuration };
	}

	D3D11Renderer_API char const * getShortName()
	{
		return "d3d11";
	}

	D3D11Renderer_API char const * getFullName()
	{
		return "Direct3D 11";
	}
}
