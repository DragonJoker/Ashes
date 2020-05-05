/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
namespace ashes
{
	template< typename T >
	Buffer< T >::Buffer( Device const & device
		, VkDeviceSize count
		, VkBufferUsageFlags usage
		, QueueShare sharingMode )
		: Buffer{ device, "Buffer", count, usage, std::move( sharingMode ) }
	{
	}

	template< typename T >
	Buffer< T >::Buffer( Device const & device
		, std::string const & debugName
		, VkDeviceSize count
		, VkBufferUsageFlags usage
		, QueueShare sharingMode )
		: m_device{ device }
		, m_buffer{ device.createBuffer( debugName
			, count * sizeof( T )
			, usage
			, std::move( sharingMode ) ) }
	{
	}
}
