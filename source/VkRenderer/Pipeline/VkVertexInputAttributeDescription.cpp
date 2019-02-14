#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkVertexInputAttributeDescription convert( ashes::VertexInputAttributeDescription const & desc )
	{
		return VkVertexInputAttributeDescription
		{
			desc.location,
			desc.binding,
			convert( desc.format ),
			desc.offset,
		};
	}
}
