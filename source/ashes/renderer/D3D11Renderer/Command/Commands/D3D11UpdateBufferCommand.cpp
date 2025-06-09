/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/D3D11UpdateBufferCommand.hpp"

#include "Buffer/D3D11Buffer.hpp"

#include "ashesd3d11_api.hpp"

#include <algorithm>

namespace ashes::D3D11_NAMESPACE
{
	UpdateBufferCommand::UpdateBufferCommand( VkDevice device
		, VkBuffer buffer
		, VkDeviceSize bufferOffset
		, ArrayView< uint8_t const > const & data )
		: CommandBase{ device }
		, m_buffer{ buffer }
		, m_memory{ get( m_buffer )->getMemory() }
		, m_memoryOffset{ bufferOffset + get( m_buffer )->getMemoryOffset() }
		, m_data{ data.begin(), data.end() }
		, m_mappable{ checkFlag( get( m_memory )->getMemoryPropertyFlags(), VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT ) }
	{
		m_dstBox.bottom = 1u;
		m_dstBox.left = UINT( bufferOffset );
		m_dstBox.right = UINT( bufferOffset + m_data.size() );
		m_dstBox.back = 1u;
	}

	void UpdateBufferCommand::apply( Context const & context )const
	{
		if ( m_mappable )
		{
			if ( void * data{};
				VK_SUCCESS == get( m_memory )->lock( m_memoryOffset, m_data.size(), 0u, &data ) )
			{
				std::memcpy( data, m_data.data(), m_data.size() );
				get( m_memory )->flush( m_memoryOffset, m_data.size() );
				get( m_memory )->unlock();
			}
		}
		else
		{
			context.context->UpdateSubresource( get( m_buffer )->getResource()
				, 0u
				, &m_dstBox
				, m_data.data()
				, UINT( m_data.size() )
				, 0u );
		}
	}

	CommandPtr UpdateBufferCommand::clone()const
	{
		return std::make_unique< UpdateBufferCommand >( *this );
	}
}
