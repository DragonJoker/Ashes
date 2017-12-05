/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#include "FormatGetter.hpp"

namespace renderer
{
	inline AttributeBase::AttributeBase( VertexLayout & layout
		, AttributeFormat format
		, uint32_t location
		, uint32_t offset )
		: m_format{ format }
		, m_location{ location }
		, m_offset{ offset }
	{
	}

	template< typename T >
	inline Attribute< T >::Attribute( VertexLayout & layout
		, uint32_t location
		, uint32_t offset )
		: m_attribute{ layout.createAttribute( location
			, details::FormatGetter< T >::value
			, offset ) }
	{
	}
}
