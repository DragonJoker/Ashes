/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "AshesRenderer/Core/Device.hpp"

namespace ashes
{
	template< typename T >
	UniformBuffer< T >::UniformBuffer( Device const & device
		, uint32_t count
		, BufferTargets target )
		: m_ubo{ device, count, sizeof( T ), target }
		, m_data( count, T{} )
	{
	}
}
