#include "Ashes/Miscellaneous/QueryPool.hpp"

#include "Ashes/Core/Device.hpp"

namespace ashes
{
	QueryPool::QueryPool( Device const & device
		, QueryType type
		, uint32_t count
		, QueryPipelineStatisticFlags pipelineStatistics )
		: m_device{ device }
		, m_type{ type }
		, m_count{ count }
		, m_pipelineStatistics{ pipelineStatistics }
	{
		registerObject( m_device, "QueryPool", this );
	}
	
	QueryPool::~QueryPool()
	{
		unregisterObject( m_device, this );
	}
}
