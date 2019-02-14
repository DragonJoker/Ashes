#include "Miscellaneous/TestQueryPool.hpp"

#include "Core/TestDevice.hpp"

namespace test_renderer
{
	QueryPool::QueryPool( Device const & device
		, ashes::QueryType type
		, uint32_t count
		, ashes::QueryPipelineStatisticFlags pipelineStatistics )
		: ashes::QueryPool{ device, type, count, pipelineStatistics }
	{
	}

	QueryPool::~QueryPool()
	{
	}

	void QueryPool::getResults( uint32_t firstQuery
		, uint32_t queryCount
		, uint32_t stride
		, ashes::QueryResultFlags flags
		, ashes::UInt32Array & data )const
	{
	}

	void QueryPool::getResults( uint32_t firstQuery
		, uint32_t queryCount
		, uint32_t stride
		, ashes::QueryResultFlags flags
		, ashes::UInt64Array & data )const
	{
	}
}
