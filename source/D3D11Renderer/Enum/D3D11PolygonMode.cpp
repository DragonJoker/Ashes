#include "D3D11RendererPrerequisites.hpp"

namespace ashes::d3d11
{
	D3D11_FILL_MODE convert( VkPolygonMode const & value )
	{
		switch ( value )
		{
		case VK_POLYGON_MODE_FILL:
			return D3D11_FILL_SOLID;

		case VK_POLYGON_MODE_LINE:
			return D3D11_FILL_WIREFRAME;

		case VK_POLYGON_MODE_POINT:
			return D3D11_FILL_MODE( 1 );

		default:
			assert( false && "Unsupported polygon mode" );
			return D3D11_FILL_SOLID;
		}
	}
}
