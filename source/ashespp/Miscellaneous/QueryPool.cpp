#include "ashespp/Miscellaneous/QueryPool.hpp"

#include "ashespp/Core/Device.hpp"

namespace ashes
{
	QueryPool::QueryPool( Device const & device
		, VkQueryType type
		, uint32_t count
		, VkQueryPipelineStatisticFlags pipelineStatistics )
		: QueryPool{ device, "QueryPool", type, count, pipelineStatistics }
	{
	}

	QueryPool::QueryPool( Device const & device
		, std::string const & debugName
		, VkQueryType type
		, uint32_t count
		, VkQueryPipelineStatisticFlags pipelineStatistics )
		: VkObject{ debugName }
		, m_device{ device }
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
			, m_device.getAllocationCallbacks()
			, &m_internal );
		checkError( res, "QueryPool creation" );
		registerObject( m_device, debugName, *this );
	}
	
	QueryPool::~QueryPool()
	{
		unregisterObject( m_device, *this );
		m_device.vkDestroyQueryPool( m_device
			, m_internal
			, m_device.getAllocationCallbacks() );
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
			, stride ? stride : sizeof( uint32_t )
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
			, stride ? stride : sizeof( uint64_t )
			, flags | VK_QUERY_RESULT_64_BIT );
		checkError( res, "QueryPool results retrieval" );
	}
}
