#include "VkQueryPool.hpp"

#include "Core/VkDevice.hpp"

namespace vk_renderer
{
	QueryPool::QueryPool( Device const & device
		, renderer::QueryType type
		, uint32_t count
		, renderer::QueryPipelineStatisticFlags pipelineStatistics )
		: renderer::QueryPool{ device, type, count, pipelineStatistics }
		, m_device{ device }
	{
		VkQueryPoolCreateInfo createInfo
		{
			VK_STRUCTURE_TYPE_QUERY_POOL_CREATE_INFO,
			nullptr,
			0u,
			convert( type ),
			count,
			convert( pipelineStatistics )
		};
		DEBUG_DUMP( createInfo );
		auto res = m_device.vkCreateQueryPool( device
			, &createInfo
			, nullptr
			, &m_pool );
		checkError( res, "QueryPool creation" );
	}

	QueryPool::~QueryPool()
	{
		m_device.vkDestroyQueryPool( m_device
			, m_pool
			, nullptr );
	}

	void QueryPool::getResults( uint32_t firstQuery
		, uint32_t queryCount
		, uint32_t stride
		, renderer::QueryResultFlags flags
		, renderer::UInt32Array & data )const
	{
		auto res = m_device.vkGetQueryPoolResults( m_device
			, m_pool
			, firstQuery
			, queryCount
			, data.size() * sizeof( uint32_t )
			, data.data()
			, stride
			, convert( flags ) );
		checkError( res, "QueryPool results retrieval" );
	}

	void QueryPool::getResults( uint32_t firstQuery
		, uint32_t queryCount
		, uint32_t stride
		, renderer::QueryResultFlags flags
		, renderer::UInt64Array & data )const
	{
		auto res = m_device.vkGetQueryPoolResults( m_device
			, m_pool
			, firstQuery
			, queryCount
			, data.size() * sizeof( uint64_t )
			, data.data()
			, stride
			, convert( flags ) | VK_QUERY_RESULT_64_BIT );
		checkError( res, "QueryPool results retrieval" );
	}
}
