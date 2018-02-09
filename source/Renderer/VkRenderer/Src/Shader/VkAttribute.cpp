#include "Shader/VkAttribute.hpp"

#include "Pipeline/VkVertexLayout.hpp"

namespace vk_renderer
{
	Attribute::Attribute( VertexLayout const & layout
		, renderer::AttributeFormat format
		, uint32_t location
		, uint32_t offset
		, uint32_t inputRate )
		: renderer::AttributeBase{ layout, format, location, offset, inputRate }
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
