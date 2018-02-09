#include "Pipeline/VkVertexLayout.hpp"

#include "Shader/VkAttribute.hpp"

namespace vk_renderer
{
	VertexLayout::VertexLayout( uint32_t bindingSlot
		, uint32_t stride
		, renderer::VertexInputRate inputRate )
		: renderer::VertexLayout{ bindingSlot, stride, inputRate }
		, m_bindingDescription
		{
			bindingSlot,
			stride,
			convert( inputRate )
		}
	{
	}

	renderer::AttributeBase VertexLayout::createAttribute( uint32_t location
		, renderer::AttributeFormat format
		, uint32_t offset
		, uint32_t divisor )
	{
		m_attributes.emplace_back( *this
			, format
			, location
			, offset
			, divisor );
		assert( m_bindingDescription.stride >= offset + getSize( format ) );
		return m_attributes.back();
	}
}
