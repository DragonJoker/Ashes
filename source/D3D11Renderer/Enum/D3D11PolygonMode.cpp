#include "D3D11RendererPrerequisites.hpp"

namespace d3d11_renderer
{
	D3D11_FILL_MODE convert( ashes::PolygonMode const & value )
	{
		switch ( value )
		{
		case ashes::PolygonMode::eFill:
			return D3D11_FILL_SOLID;

		case ashes::PolygonMode::eLine:
			return D3D11_FILL_WIREFRAME;

		case ashes::PolygonMode::ePoint:
			return D3D11_FILL_MODE( 1 );

		default:
			assert( false && "Unsupported polygon mode" );
			return D3D11_FILL_SOLID;
		}
	}
}
