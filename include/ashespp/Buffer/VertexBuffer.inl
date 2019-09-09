/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
namespace ashes
{
	template< typename T >
	inline VertexBuffer< T >::VertexBuffer( Device const & device
		, VkDeviceSize count
		, VkBufferUsageFlags usage
		, QueueShare sharingMode )
		: VertexBufferBase{ device
			, count * sizeof( T )
			, usage
			, std::move( sharingMode ) }
	{
	}
}
