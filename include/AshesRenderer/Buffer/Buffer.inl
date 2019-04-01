/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
namespace ashes
{
	template< typename T >
	Buffer< T >::Buffer( Device const & device
		, uint32_t count
		, BufferTargets target )
		: m_device{ device }
		, m_buffer{ device.createBuffer( uint32_t( count * sizeof( T ) )
			, target ) }
	{
	}
}
