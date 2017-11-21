#include "Attribute.hpp"

namespace renderer
{
	template< typename AttributeType >
	inline Attribute< AttributeType > VertexLayout::createAttribute( uint32_t location
		, uint32_t offset )
	{
		return Attribute< AttributeType >{ *m_layout
			, location
			, offset };
	}
}
