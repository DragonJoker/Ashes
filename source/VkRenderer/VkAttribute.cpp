#include "VkAttribute.hpp"

#include "VkVertexLayout.hpp"

namespace vk_renderer
{
	AttributeBase::AttributeBase( renderer::VertexLayout & layout
		, renderer::AttributeFormat format
		, uint32_t location
		, uint32_t offset )
		: renderer::AttributeBase{ layout, format, location, offset }
		, m_attribute{ static_cast< VertexLayout const & >( layout ).getLayout()->createAttribute( location
			, convert( format )
			, offset
			, getSize( format ) ) }
	{
	}
}
