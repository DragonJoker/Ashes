/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
namespace ashes
{
	template< typename T >
	Buffer< T >::Buffer( Device const & device
		, VkDeviceSize count
		, VkBufferUsageFlags usage )
		: m_device{ device }
		, m_buffer{ device.createBuffer( count * sizeof( T )
			, usage ) }
	{
	}
}
