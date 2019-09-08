/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "ashespp/Core/Device.hpp"

namespace ashes
{
	template< typename T >
	UniformBuffer< T >::UniformBuffer( Device const & device
		, VkDeviceSize count
		, VkBufferUsageFlags usage )
		: m_ubo{ device, count, sizeof( T ), usage }
		, m_data( count, T{} )
	{
	}
}
