#include "Miscellaneous/D3D11QueryPool.hpp"

#include "Core/D3D11Device.hpp"

namespace ashes::d3d11
{
	namespace
	{
		D3D11_QUERY convert( VkQueryType type )
		{
			switch ( type )
			{
			case VK_QUERY_TYPE_OCCLUSION:
				return D3D11_QUERY_OCCLUSION;

			case VK_QUERY_TYPE_PIPELINE_STATISTICS:
				return D3D11_QUERY_PIPELINE_STATISTICS;

			case VK_QUERY_TYPE_TIMESTAMP:
				return D3D11_QUERY_TIMESTAMP;

			default:
				assert( false );
				return D3D11_QUERY_TIMESTAMP;
			}
		}

		UINT64 getPipelineStatistic( uint32_t index
			, D3D10_QUERY_DATA_PIPELINE_STATISTICS const & stats )
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

	QueryPool::QueryPool( Device const & device
		, VkQueryType type
		, uint32_t count
		, ashes::QueryPipelineStatisticFlags pipelineStatistics )
		: ashes::QueryPool{ device, type, count, pipelineStatistics }
		, m_device{ device }
	{
		D3D11_QUERY_DESC desc;
		desc.MiscFlags = 0u;
		desc.Query = convert( type );
		m_queries.resize( count );

		for ( auto & query : m_queries )
		{
			auto hr = device.getDevice()->CreateQuery( &desc, &query );

			if ( checkError( m_device, hr, "CreateQuery" ) )
			{
				dxDebugName( query, Query );
			}
		}

		switch ( type )
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
			m_data.resize( sizeof( D3D10_QUERY_DATA_PIPELINE_STATISTICS ) );
			getUint32 = [this]( uint32_t index )
			{
				D3D10_QUERY_DATA_PIPELINE_STATISTICS data = *reinterpret_cast< D3D10_QUERY_DATA_PIPELINE_STATISTICS * >( m_data.data() );
				return uint32_t( getPipelineStatistic( index, data ) );
			};
			getUint64 = [this]( uint32_t index )
			{
				D3D10_QUERY_DATA_PIPELINE_STATISTICS data = *reinterpret_cast< D3D10_QUERY_DATA_PIPELINE_STATISTICS * >( m_data.data() );
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

	QueryPool::~QueryPool()
	{
		for ( auto & query : m_queries )
		{
			safeRelease( query );
		}
	}

	void QueryPool::getResults( uint32_t firstQuery
		, uint32_t queryCount
		, uint32_t stride
		, VkQueryResultFlags flags
		, ashes::UInt32Array & datas )const
	{
		auto max = firstQuery + queryCount;
		assert( max <= m_queries.size() );
		ID3D11DeviceContext * context;
		m_device.getDevice()->GetImmediateContext( &context );

		for ( auto i = firstQuery; i < max; ++i )
		{
			HRESULT hr;

			do
			{
				hr = context->GetData( m_queries[i]
					, m_data.data()
					, UINT( m_data.size() )
					, 0u );
			}
			while ( hr == S_FALSE || !checkFlag( flags, VK_QUERY_RESULT_WAIT_BIT ) );

			if ( checkError( m_device, hr, "GetData" ) )
			{
				datas[i - firstQuery] = getUint32( i );
			}
		}

		safeRelease( context );
	}

	void QueryPool::getResults( uint32_t firstQuery
		, uint32_t queryCount
		, uint32_t stride
		, VkQueryResultFlags flags
		, ashes::UInt64Array & datas )const
	{
		auto max = firstQuery + queryCount;
		assert( max <= m_queries.size() );
		ID3D11DeviceContext * context;
		m_device.getDevice()->GetImmediateContext( &context );

		for ( auto i = firstQuery; i < max; ++i )
		{
			HRESULT hr;

			do
			{
				hr = context->GetData( m_queries[i]
					, m_data.data()
					, UINT( m_data.size() )
					, 0u );
			}
			while ( hr == S_FALSE || !checkFlag( flags, VK_QUERY_RESULT_WAIT_BIT ) );

			if ( checkError( m_device, hr, "GetData" ) )
			{
				datas[i - firstQuery] = getUint64( i );
			}
		}

		safeRelease( context );
	}
}
