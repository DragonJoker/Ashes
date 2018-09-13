#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkPrimitiveTopology convert( ashes::PrimitiveTopology const & topology )
	{
		switch ( topology )
		{
		case ashes::PrimitiveTopology::ePointList:
			return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;

		case ashes::PrimitiveTopology::eLineList:
			return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;

		case ashes::PrimitiveTopology::eLineStrip:
			return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;

		case ashes::PrimitiveTopology::eTriangleList:
			return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

		case ashes::PrimitiveTopology::eTriangleStrip:
			return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;

		case ashes::PrimitiveTopology::eTriangleFan:
			return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN;

		case ashes::PrimitiveTopology::eLineListWithAdjacency:
			return VK_PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY;

		case ashes::PrimitiveTopology::eLineStripWithAdjacency:
			return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY;

		case ashes::PrimitiveTopology::eTriangleListWithAdjacency:
			return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY;

		case ashes::PrimitiveTopology::eTriangleStripWithAdjacency:
			return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY;

		case ashes::PrimitiveTopology::ePatchList:
			return VK_PRIMITIVE_TOPOLOGY_PATCH_LIST;

		default:
			assert( false );
			return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		}
	}
}
