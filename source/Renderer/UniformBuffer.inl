#include "Device.hpp"

namespace renderer
{
	template< typename T >
	UniformBuffer< T >::UniformBuffer( Device const & device
		, uint32_t count
		, BufferTarget target
		, MemoryPropertyFlags flags )
		: m_device{ device }
		, m_buffer{ device.getDevice().createUniformBuffer( count * sizeof( T )
			, convert( target )
			, convert( flags ) ) }
		, m_data( count, T{} )
	{
	}
}
