#include "AshesPP/Miscellaneous/QueryPool.hpp"

#include "AshesPP/Core/Device.hpp"

namespace ashes
{
	QueryPool::QueryPool( Device const & device
		, VkQueryType type
		, uint32_t count
		, VkQueryPipelineStatisticFlags pipelineStatistics )
		: m_device{ device }
		, m_type{ type }
		, m_count{ count }
		, m_pipelineStatistics{ pipelineStatistics }
	{
		VkQueryPoolCreateInfo createInfo
		{
			VK_STRUCTURE_TYPE_QUERY_POOL_CREATE_INFO,
			nullptr,
			0u,
			type,
			count,
			pipelineStatistics
		};
		DEBUG_DUMP( createInfo );
		auto res = m_device.vkCreateQueryPool( device
			, &createInfo
			, nullptr
			, &m_internal );
		checkError( res, "QueryPool creation" );
		registerObject( m_device, "QueryPool", this );
	}
	
	QueryPool::~QueryPool()
	{
		unregisterObject( m_device, this );
		m_device.vkDestroyQueryPool( m_device
			, m_internal
			, nullptr );
	}

	void QueryPool::getResults( uint32_t firstQuery
		, uint32_t queryCount
		, uint32_t stride
		, VkQueryResultFlags flags
		, UInt32Array & data )const
	{
		auto res = m_device.vkGetQueryPoolResults( m_device
			, m_internal
			, firstQuery
			, queryCount
			, data.size() * sizeof( uint32_t )
			, data.data()
			, stride
			, flags );
		checkError( res, "QueryPool results retrieval" );
	}

	void QueryPool::getResults( uint32_t firstQuery
		, uint32_t queryCount
		, uint32_t stride
		, VkQueryResultFlags flags
		, UInt64Array & data )const
	{
		auto res = m_device.vkGetQueryPoolResults( m_device
			, m_internal
			, firstQuery
			, queryCount
			, data.size() * sizeof( uint64_t )
			, data.data()
			, stride
			, flags | VK_QUERY_RESULT_64_BIT );
		checkError( res, "QueryPool results retrieval" );
	}
}
