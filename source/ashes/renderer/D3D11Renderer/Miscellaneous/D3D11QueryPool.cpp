#include "Miscellaneous/D3D11QueryPool.hpp"

#include "Core/D3D11Device.hpp"

#include "ashesd3d11_api.hpp"

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
			, D3D11_QUERY_DATA_PIPELINE_STATISTICS const & stats )
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
			case 8:
				return stats.HSInvocations;
			case 9:
				return stats.DSInvocations;
			case 10:
				return stats.CSInvocations;
			}

			return 0ull;
		}

		uint32_t adjustQueryCount( uint32_t count
			, VkQueryType type
			, VkQueryPipelineStatisticFlags pipelineStatistics )
		{
			if ( type != VK_QUERY_TYPE_PIPELINE_STATISTICS )
			{
				return count;
			}

			return 1u;
		}
	}

	QueryPool::QueryPool( VkDevice device
		, VkQueryPoolCreateInfo createInfo )
		: m_device{ device }
		, m_createInfo{ std::move( createInfo ) }
	{
		D3D11_QUERY_DESC desc;
		desc.MiscFlags = 0u;
		desc.Query = convert( m_createInfo.queryType );
		m_queries.resize( adjustQueryCount( m_createInfo.queryCount, m_createInfo.queryType, m_createInfo.pipelineStatistics ) );

		for ( auto & query : m_queries )
		{
			auto hr = get( m_device )->getDevice()->CreateQuery( &desc, &query );

			if ( checkError( m_device, hr, "CreateQuery" ) )
			{
				dxDebugName( query, Query );
			}
		}

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
			m_data.resize( sizeof( D3D11_QUERY_DATA_PIPELINE_STATISTICS ) );
			getUint32 = [this]( uint32_t index )
			{
				D3D11_QUERY_DATA_PIPELINE_STATISTICS data = *reinterpret_cast< D3D11_QUERY_DATA_PIPELINE_STATISTICS * >( m_data.data() );
				return uint32_t( getPipelineStatistic( index, data ) );
			};
			getUint64 = [this]( uint32_t index )
			{
				D3D11_QUERY_DATA_PIPELINE_STATISTICS data = *reinterpret_cast< D3D11_QUERY_DATA_PIPELINE_STATISTICS * >( m_data.data() );
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

		default:
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

	VkResult QueryPool::getResults( uint32_t firstQuery
		, uint32_t queryCount
		, VkDeviceSize stride
		, VkQueryResultFlags flags
		, VkDeviceSize dataSize
		, void * data )const
	{
		if ( m_createInfo.queryType == VK_QUERY_TYPE_PIPELINE_STATISTICS )
		{
			return getPipelineStatisticsResults( firstQuery
				, queryCount
				, stride
				, flags
				, dataSize
				, data );
		}

		return getOtherResults( firstQuery
			, queryCount
			, stride
			, flags
			, dataSize
			, data );
	}

	VkResult QueryPool::getPipelineStatisticsResults( uint32_t firstQuery
		, uint32_t queryCount
		, VkDeviceSize stride
		, VkQueryResultFlags flags
		, VkDeviceSize dataSize
		, void * data )const
	{
		auto max = firstQuery + queryCount;
		assert( max <= m_queries.size() );
		auto context{ get( m_device )->getImmediateContext() };
		VkResult result = VK_SUCCESS;

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
			while ( hr == S_FALSE );

			if ( hr == S_FALSE )
			{
				result = VK_INCOMPLETE;
			}
			else if ( checkError( m_device, hr, "GetData" ) )
			{
				auto buffer = reinterpret_cast< uint8_t * >( data );
				size_t size = 0u;

				if ( checkFlag( flags, VK_QUERY_RESULT_64_BIT ) )
				{
					auto resmax = ( m_createInfo.queryType == VK_QUERY_TYPE_PIPELINE_STATISTICS
						? uint32_t( m_data.size() / sizeof( uint64_t ) )
						: queryCount );
					stride = stride
						? stride
						: sizeof( uint64_t );

					for ( uint32_t resi = 0; resi < resmax && size < dataSize; ++resi )
					{
						*reinterpret_cast< uint64_t * >( buffer ) = getUint64( resi );
						buffer += stride;
						size += stride;
					}
				}
				else
				{
					auto resmax = ( m_createInfo.queryType == VK_QUERY_TYPE_PIPELINE_STATISTICS
						? uint32_t( m_data.size() / sizeof( uint32_t ) )
						: queryCount );
					stride = stride
						? stride
						: sizeof( uint32_t );

					for ( uint32_t resi = 0; resi < resmax && size < dataSize; ++resi )
					{
						*reinterpret_cast< uint32_t * >( buffer ) = getUint32( resi );
						buffer += stride;
						size += stride;
					}
				}
			}
		}

		return result;
	}

	VkResult QueryPool::getOtherResults( uint32_t firstQuery
		, uint32_t queryCount
		, VkDeviceSize stride
		, VkQueryResultFlags flags
		, VkDeviceSize dataSize
		, void * data )const
	{
		auto max = firstQuery + queryCount;
		assert( max <= m_queries.size() );
		auto context{ get( m_device )->getImmediateContext() };
		VkResult result = VK_SUCCESS;
		stride = stride
			? stride
			: ( checkFlag( flags, VK_QUERY_RESULT_64_BIT )
				? sizeof( uint64_t )
				: sizeof( uint32_t ) );
		auto buffer = reinterpret_cast< uint8_t * >( data );

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
			while ( hr == S_FALSE );

			if ( hr == S_FALSE )
			{
				result = VK_INCOMPLETE;
			}
			else if ( checkError( m_device, hr, "GetData" ) )
			{
				if ( checkFlag( flags, VK_QUERY_RESULT_64_BIT ) )
				{
					*reinterpret_cast< uint64_t * >( buffer ) = getUint64( 0 );
					buffer += stride;
				}
				else
				{
					*reinterpret_cast< uint32_t * >( buffer ) = getUint32( 0 );
					buffer += stride;
				}
			}
		}

		return result;
	}
}
