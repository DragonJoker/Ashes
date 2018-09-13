/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Core/Device.hpp"

namespace ashes
{
	template< typename T >
	UniformBuffer< T >::UniformBuffer( Device const & device
		, uint32_t count
		, BufferTargets target
		, MemoryPropertyFlags flags )
		: m_ubo{ device.createUniformBuffer( count
			, sizeof( T )
			, target
			, flags ) }
		, m_data( count, T{} )
	{
	}
}
