/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/D3D11UpdateBufferCommand.hpp"

#include "Buffer/D3D11Buffer.hpp"

#include "ashesd3d11_api.hpp"

#include <algorithm>

namespace ashes::d3d11
{
	UpdateBufferCommand::UpdateBufferCommand( VkDevice device
		, VkBuffer buffer
		, VkDeviceSize bufferOffset
		, ArrayView< uint8_t const > const & data )
		: CommandBase{ device }
		, m_memory{ get( buffer )->getMemory() }
		, m_memoryOffset{ get( buffer )->getMemoryOffset() }
		, m_data{ data.begin(), data.end() }
	{
	}

	void UpdateBufferCommand::apply( Context const & context )const
	{
		void * data;

		if ( VK_SUCCESS == get( m_memory )->lock( m_memoryOffset, m_data.size(), 0u, &data ) )
		{
			std::memcpy( data, m_data.data(), m_data.size() );
			get( m_memory )->flush( m_memoryOffset, m_data.size() );
			get( m_memory )->unlock();
		}
	}

	CommandPtr UpdateBufferCommand::clone()const
	{
		return std::make_unique< UpdateBufferCommand >( *this );
	}
}
