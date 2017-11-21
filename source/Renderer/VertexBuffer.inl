#include "Attribute.hpp"

namespace renderer
{
	template< typename T >
	inline VertexBuffer< T >::VertexBuffer( RenderingResources const & resources
		, uint32_t bindingSlot
		, uint32_t count
		, BufferTargets target
		, MemoryPropertyFlags flags )
		: m_resources{ resources }
		, m_buffer{ resources.getDevice().createVertexBuffer( count * sizeof( T )
			, convert( target )
			, convert( flags ) ) }
	{
	}

	template< typename T >
	inline VertexBuffer< T >::VertexBuffer( RenderingResources const & resources
		, uint32_t bindingSlot
		, std::vector< T > const & data
		, BufferTargets target
		, MemoryPropertyFlags flags )
		: VertexBuffer{ resources, bindingSlot, uint32_t( data.size() ), target, flags }
	{
		m_resources.copyVertexData( data
			, *m_buffer );
	}
}
