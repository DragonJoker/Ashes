/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/D3D11FillBufferCommand.hpp"

#include "Buffer/D3D11Buffer.hpp"

#include "ashesd3d11_api.hpp"

#include <cstring>

namespace ashes::d3d11
{
	FillBufferCommand::FillBufferCommand( VkDevice device
		, VkBuffer buffer
		, VkDeviceSize bufferOffset
		, VkDeviceSize size
		, uint32_t data )
		: CommandBase{ device }
		, m_memory{ get( buffer )->getMemory() }
		, m_memoryOffset{ get( buffer )->getMemoryOffset() }
		, m_size{ size }
		, m_data{ data }
	{
	}

	void FillBufferCommand::apply( Context const & context )const
	{
		uint32_t * data;

		if ( VK_SUCCESS == get( m_memory )->lock( m_memoryOffset, m_size, 0u, reinterpret_cast< void ** >( &data ) ) )
		{
			std::fill_n( data, m_size / sizeof( uint32_t ), m_data );
			get( m_memory )->flush( m_memoryOffset, m_size );
			get( m_memory )->unlock();
		}
	}

	CommandPtr FillBufferCommand::clone()const
	{
		return std::make_unique< FillBufferCommand >( *this );
	}
}
