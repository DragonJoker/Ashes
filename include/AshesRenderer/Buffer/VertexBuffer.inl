/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
namespace ashes
{
	template< typename T >
	inline VertexBuffer< T >::VertexBuffer( Device const & device
		, uint32_t count
		, BufferTargets target )
		: VertexBufferBase{ device
			, uint32_t( count * sizeof( T ) )
			, target }
	{
	}
}
