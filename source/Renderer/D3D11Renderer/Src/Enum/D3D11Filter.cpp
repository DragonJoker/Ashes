#include "D3D11RendererPrerequisites.hpp"

namespace d3d11_renderer
{
	D3D11_FILTER_TYPE convert( renderer::Filter const & filter )
	{
		switch ( filter )
		{
		case renderer::Filter::eNearest:
			return D3D11_FILTER_TYPE_POINT;

		case renderer::Filter::eLinear:
			return D3D11_FILTER_TYPE_LINEAR;

		default:
			assert( false );
			return D3D11_FILTER_TYPE_POINT;
		}
	}

}
