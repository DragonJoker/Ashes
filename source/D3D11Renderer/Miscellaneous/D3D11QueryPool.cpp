#include "Miscellaneous/D3D11QueryPool.hpp"

#include "Core/D3D11Device.hpp"

namespace d3d11_renderer
{
	namespace
	{
		D3D11_QUERY convert( ashes::QueryType type )
		{
			switch ( type )
			{
			case ashes::QueryType::eOcclusion:
				return D3D11_QUERY_OCCLUSION;

			case ashes::QueryType::ePipelineStatistics:
				return D3D11_QUERY_PIPELINE_STATISTICS;

			case ashes::QueryType::eTimestamp:
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
		, ashes::QueryType type
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

			if ( dxCheckError( hr, "CreateQuery" ) )
			{
				dxDebugName( query, Query );
			}
		}

		switch ( type )
		{
		case ashes::QueryType::eOcclusion:
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

		case ashes::QueryType::ePipelineStatistics:
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

		case ashes::QueryType::eTimestamp:
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
		, ashes::QueryResultFlags flags
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
			while ( hr == S_FALSE || !checkFlag( flags, ashes::QueryResultFlag::eWait ) );

			if ( dxCheckError( hr, "GetData" ) )
			{
				datas[i - firstQuery] = getUint32( i );
			}
		}

		safeRelease( context );
	}

	void QueryPool::getResults( uint32_t firstQuery
		, uint32_t queryCount
		, uint32_t stride
		, ashes::QueryResultFlags flags
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
			while ( hr == S_FALSE || !checkFlag( flags, ashes::QueryResultFlag::eWait ) );

			if ( dxCheckError( hr, "GetData" ) )
			{
				datas[i - firstQuery] = getUint64( i );
			}
		}

		safeRelease( context );
	}
}
