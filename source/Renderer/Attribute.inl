#include "FormatGetter.inl"

namespace renderer
{
	template< typename T >
	Attribute< T >::Attribute( vk::VertexLayout & layout
		, uint32_t location
		, uint32_t offset )
		: m_attribute{ layout.createAttribute( location
			, details::FormatGetter< T >::value
			, offset
			, uint32_t( sizeof( T ) ) ) }
	{
	}
}
