#include "GlVertexLayout.hpp"

#include "GlAttribute.hpp"

namespace gl_renderer
{
	VertexLayout::VertexLayout( uint32_t bindingSlot
		, uint32_t stride )
		: renderer::VertexLayout{ bindingSlot, stride }
	{
	}

	renderer::AttributeBase VertexLayout::createAttribute( uint32_t location
		, renderer::AttributeFormat format
		, uint32_t offset )
	{
		return m_attributes.emplace_back( *this, format, location, offset );
	}
}
