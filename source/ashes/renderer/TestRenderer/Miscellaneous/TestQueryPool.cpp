#include "Miscellaneous/TestQueryPool.hpp"

#include "Core/TestDevice.hpp"

#include "ashestest_api.hpp"

namespace ashes::test
{
	namespace
	{
		struct PipelineStatistics
		{
			uint64_t IAVertices;
			uint64_t IAPrimitives;
			uint64_t VSInvocations;
			uint64_t GSInvocations;
			uint64_t GSPrimitives;
			uint64_t CInvocations;
			uint64_t CPrimitives;
			uint64_t PSInvocations;
		};

		uint64_t getPipelineStatistic( uint32_t index
			, PipelineStatistics const & stats )
		{
			switch ( index )
			{
			case 0:
				return stats.IAVertices;
			case 1:
				return stats.IAPrimitives;
			case 2:
				return stats.VSInvocations;
			case 3:
				return stats.CInvocations;
			case 4:
				return stats.CPrimitives;
			case 5:
				return stats.PSInvocations;
			case 6:
				return stats.GSPrimitives;
			case 7:
				return stats.GSInvocations;
			}

			return 0ull;
		}
	}

	QueryPool::QueryPool( VkDevice device
		, VkQueryPoolCreateInfo createInfo )
		: m_device{ device }
		, m_createInfo{ std::move( createInfo ) }
	{
		switch ( m_createInfo.queryType )
		{
		case VK_QUERY_TYPE_OCCLUSION:
			m_data.resize( sizeof( uint64_t ) );
			getUint32 = [this]( uint32_t index )
			{
				return uint32_t( *reinterpret_cast< uint64_t * >( m_data.data() ) );
			};
			getUint64 = [this]( uint32_t index )
			{
				return *reinterpret_cast< uint64_t * >( m_data.data() );
			};
			break;

		case VK_QUERY_TYPE_PIPELINE_STATISTICS:
			m_data.resize( sizeof( PipelineStatistics ) );
			getUint32 = [this]( uint32_t index )
			{
				PipelineStatistics data = *reinterpret_cast< PipelineStatistics * >( m_data.data() );
				return uint32_t( getPipelineStatistic( index, data ) );
			};
			getUint64 = [this]( uint32_t index )
			{
				PipelineStatistics data = *reinterpret_cast< PipelineStatistics * >( m_data.data() );
				return getPipelineStatistic( index, data );
			};
			break;

		case VK_QUERY_TYPE_TIMESTAMP:
			m_data.resize( sizeof( uint64_t ) );
			getUint32 = [this]( uint32_t index )
			{
				return uint32_t( *reinterpret_cast< uint64_t * >( m_data.data() ) );
			};
			getUint64 = [this]( uint32_t index )
			{
				return *reinterpret_cast< uint64_t * >( m_data.data() );
			};
			break;
		}
	}

	VkResult QueryPool::getResults( uint32_t firstQuery
		, uint32_t queryCount
		, VkDeviceSize stride
		, VkQueryResultFlags flags
		, UInt32Array & datas )const
	{
		auto max = firstQuery + queryCount;
		VkResult result = VK_SUCCESS;

		for ( auto i = firstQuery; i < max; ++i )
		{
			datas[i - firstQuery] = getUint32( i );
		}

		return result;
	}

	VkResult QueryPool::getResults( uint32_t firstQuery
		, uint32_t queryCount
		, VkDeviceSize stride
		, VkQueryResultFlags flags
		, UInt64Array & datas )const
	{
		auto max = firstQuery + queryCount;
		VkResult result = VK_SUCCESS;

		for ( auto i = firstQuery; i < max; ++i )
		{
			datas[i - firstQuery] = getUint64( i );
		}

		return result;
	}
}
