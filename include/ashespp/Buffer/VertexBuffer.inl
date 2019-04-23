/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
namespace ashes
{
	template< typename T >
	inline VertexBuffer< T >::VertexBuffer( Device const & device
		, VkDeviceSize count
		, VkBufferUsageFlags target )
		: VertexBufferBase{ device
			, count * sizeof( T )
			, target }
	{
	}
}
