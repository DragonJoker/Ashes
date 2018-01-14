#include "VkAttribute.hpp"

#include "VkVertexLayout.hpp"

namespace vk_renderer
{
	Attribute::Attribute( VertexLayout & layout
		, renderer::AttributeFormat format
		, uint32_t location
		, uint32_t offset )
		: renderer::AttributeBase{ layout, format, location, offset }
		, m_description
		{
			location,
			layout.getBindingSlot(),
			convert( format ),
			offset
		}
	{
	}
}
