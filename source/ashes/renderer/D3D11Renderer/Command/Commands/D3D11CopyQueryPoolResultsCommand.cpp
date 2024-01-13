/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/D3D11CopyQueryPoolResultsCommand.hpp"

#include "Buffer/D3D11Buffer.hpp"
#include "Miscellaneous/D3D11QueryPool.hpp"

#include "ashesd3d11_api.hpp"

#include <cstring>

namespace ashes::d3d11
{
	CopyQueryPoolResultsCommand::CopyQueryPoolResultsCommand( VkDevice device
		, VkQueryPool queryPool
		, uint32_t firstQuery
		, uint32_t queryCount
		, VkBuffer dstBuffer
		, VkDeviceSize dstOffset
		, VkDeviceSize stride
		, VkQueryResultFlags flags )
		: CommandBase{ device }
		, m_queryPool{ queryPool }
		, m_firstQuery{ firstQuery }
		, m_queryCount{ queryCount }
		, m_dstBuffer{ dstBuffer }
		, m_dstMemory{ get( m_dstBuffer )->getMemory() }
		, m_dstOffset{ dstOffset + get( m_dstBuffer )->getMemoryOffset() }
		, m_stride{ stride }
		, m_flags{ flags }
		, m_mappable{ checkFlag( get( m_dstMemory )->getMemoryPropertyFlags(), VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT ) }
	{
		m_data.resize( get( m_dstBuffer )->getSize() );
		m_dstBox.bottom = 1u;
		m_dstBox.left = UINT( dstOffset );
		m_dstBox.right = UINT( dstOffset + m_data.size() );
		m_dstBox.back = 1u;
	}

	void CopyQueryPoolResultsCommand::apply( Context const & context )const
	{
		get( m_queryPool )->getResults( m_firstQuery
			, m_queryCount
			, m_stride
			, m_flags
			, m_data.size()
			, m_data.data() );

		if ( m_mappable )
		{
			if ( void * data;
				get( m_dstMemory )->lock( m_dstOffset, m_data.size(), 0u, &data ) == VK_SUCCESS )
			{
				std::memcpy( data, m_data.data(), m_data.size() );
				get( m_dstMemory )->flush( m_dstOffset, m_data.size() );
				get( m_dstMemory )->unlock();
			}
		}
		else
		{
			context.context->UpdateSubresource( get( m_dstBuffer )->getResource()
				, 0u
				, &m_dstBox
				, m_data.data()
				, UINT( m_data.size() )
				, 0u );
		}
	}

	CommandPtr CopyQueryPoolResultsCommand::clone()const
	{
		return std::make_unique< CopyQueryPoolResultsCommand >( *this );
	}
}
