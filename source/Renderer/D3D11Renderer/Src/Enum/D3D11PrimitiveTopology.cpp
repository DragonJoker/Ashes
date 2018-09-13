#include "D3D11RendererPrerequisites.hpp"

namespace d3d11_renderer
{
	D3D11_PRIMITIVE_TOPOLOGY convert( renderer::PrimitiveTopology const & topology )
	{
		switch ( topology )
		{
		case renderer::PrimitiveTopology::ePointList:
			return D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;

		case renderer::PrimitiveTopology::eLineList:
			return D3D11_PRIMITIVE_TOPOLOGY_LINELIST;

		case renderer::PrimitiveTopology::eLineStrip:
			return D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;

		case renderer::PrimitiveTopology::eTriangleList:
			return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

		case renderer::PrimitiveTopology::eTriangleStrip:
			return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;

		case renderer::PrimitiveTopology::eLineListWithAdjacency:
			return D3D11_PRIMITIVE_TOPOLOGY_LINELIST_ADJ;

		case renderer::PrimitiveTopology::eLineStripWithAdjacency:
			return D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ;

		case renderer::PrimitiveTopology::eTriangleListWithAdjacency:
			return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ;

		case renderer::PrimitiveTopology::eTriangleStripWithAdjacency:
			return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ;

		case renderer::PrimitiveTopology::ePatchList:
			return D3D11_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST;

		default:
			assert( false );
			return D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
		}
	}
}
