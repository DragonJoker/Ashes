#include "Attribute.hpp"
#include "StagingBuffer.hpp"

namespace renderer
{
	template< typename T >
	inline VertexBuffer< T >::VertexBuffer( Device const & device
		, uint32_t bindingSlot
		, uint32_t count
		, BufferTargets target
		, MemoryPropertyFlags flags )
		: VertexBufferBase{ device
			, bindingSlot
			, uint32_t( count * sizeof( T ) )
			, target
			, flags }
	{
	}
}
