#include "PrimitiveTopology.hpp"

namespace renderer
{
	VkPrimitiveTopology convert( PrimitiveTopology const & topology )
	{
		switch ( topology )
		{
		case PrimitiveTopology::ePointList:
			return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;

		case PrimitiveTopology::eLineList:
			return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;

		case PrimitiveTopology::eLineStrip:
			return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;

		case PrimitiveTopology::eTriangleList:
			return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

		case PrimitiveTopology::eTriangleStrip:
			return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;

		case PrimitiveTopology::eTriangleFan:
			return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN;

		case PrimitiveTopology::eLineListWithAdjacency:
			return VK_PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY;

		case PrimitiveTopology::eLineStripWithAdjacency:
			return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY;

		case PrimitiveTopology::eTriangleListWithAdjacency:
			return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY;

		case PrimitiveTopology::eTriangleStripWithAdjacency:
			return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY;

		case PrimitiveTopology::ePatchList:
			return VK_PRIMITIVE_TOPOLOGY_PATCH_LIST;

		default:
			assert( false );
			return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		}
	}
}
