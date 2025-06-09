/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/D3D11FillBufferCommand.hpp"

#include "Buffer/D3D11Buffer.hpp"

#include "ashesd3d11_api.hpp"

#include <cstring>

namespace ashes::D3D11_NAMESPACE
{
	FillBufferCommand::FillBufferCommand( VkDevice device
		, VkBuffer buffer
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
		if ( uint32_t * data{};
			VK_SUCCESS == get( m_memory )->lock( m_memoryOffset, m_size, 0u, reinterpret_cast< void ** >( &data ) ) )
		{
			std::fill_n( data, m_size / sizeof( uint32_t ), m_data );
			get( m_memory )->flush( context.context, m_memoryOffset, m_size );
			get( m_memory )->unlock( context.context );
		}
	}

	CommandPtr FillBufferCommand::clone()const
	{
		return std::make_unique< FillBufferCommand >( *this );
	}
}
