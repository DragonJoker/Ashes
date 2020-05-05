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
		: VertexBuffer{ device, "VertexBuffer", count, usage, std::move( sharingMode ) }
	{
	}

	template< typename T >
	inline VertexBuffer< T >::VertexBuffer( Device const & device
		, std::string const & debugName
		, VkDeviceSize count
		, VkBufferUsageFlags usage
		, QueueShare sharingMode )
		: VertexBufferBase{ device
			, debugName
			, count * sizeof( T )
			, usage
			, std::move( sharingMode ) }
	{
	}
}
