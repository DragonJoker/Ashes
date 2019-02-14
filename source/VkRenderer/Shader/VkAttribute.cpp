#include "Shader/VkAttribute.hpp"

#include <Ashes/Pipeline/VertexLayout.hpp>

namespace vk_renderer
{
	VkVertexInputAttributeDescription convert( ashes::Attribute const & value )
	{
		return VkVertexInputAttributeDescription
		{
			value.location,
			value.layout.getBindingSlot(),
			convert( value.format ),
			value.offset
		};
	}
}
