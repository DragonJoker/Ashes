#include "D3D11RendererPrerequisites.hpp"

namespace d3d11_renderer
{
	D3D11_PRIMITIVE_TOPOLOGY convert( ashes::PrimitiveTopology const & topology )
	{
		switch ( topology )
		{
		case ashes::PrimitiveTopology::ePointList:
			return D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;

		case ashes::PrimitiveTopology::eLineList:
			return D3D11_PRIMITIVE_TOPOLOGY_LINELIST;

		case ashes::PrimitiveTopology::eLineStrip:
			return D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;

		case ashes::PrimitiveTopology::eTriangleList:
			return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

		case ashes::PrimitiveTopology::eTriangleStrip:
			return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;

		case ashes::PrimitiveTopology::eLineListWithAdjacency:
			return D3D11_PRIMITIVE_TOPOLOGY_LINELIST_ADJ;

		case ashes::PrimitiveTopology::eLineStripWithAdjacency:
			return D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ;

		case ashes::PrimitiveTopology::eTriangleListWithAdjacency:
			return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ;

		case ashes::PrimitiveTopology::eTriangleStripWithAdjacency:
			return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ;

		case ashes::PrimitiveTopology::ePatchList:
			return D3D11_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST;

		default:
			assert( false );
			return D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
		}
	}
}
