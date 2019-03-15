#include "Core/D3D11CreateInstance.hpp"
#include "Core/D3D11Instance.hpp"

extern "C"
{
	D3D11Renderer_API ashes::Instance * createInstance( ashes::Instance::Configuration const & configuration )
	{
		return new d3d11_renderer::Instance{ configuration };
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
