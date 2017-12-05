#include "VkVertexLayout.hpp"

#include "VkAttribute.hpp"

namespace vk_renderer
{
	VertexLayout::VertexLayout( uint32_t bindingSlot
		, uint32_t stride )
		: renderer::VertexLayout{ bindingSlot, stride }
		, m_layout{ std::make_shared< vk::VertexLayout >( bindingSlot
			, stride ) }
	{
	}

	renderer::AttributeBase VertexLayout::createAttribute( uint32_t location
		, renderer::AttributeFormat format
		, uint32_t offset )
	{
		return AttributeBase{ *this, format, location, offset };
	}
}
