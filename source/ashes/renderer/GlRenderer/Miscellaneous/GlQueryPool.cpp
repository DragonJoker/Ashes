#include "Miscellaneous/GlQueryPool.hpp"

#include "Core/GlDevice.hpp"
#include "Miscellaneous/GlCallLogger.hpp"

#include "ashesgl_api.hpp"

namespace ashes::gl
{
	QueryPool::QueryPool( VkDevice device
		, VkQueryPoolCreateInfo createInfo )
		: m_device{ device }
		, m_flags{ createInfo.flags }
		, m_queryType{ createInfo.queryType }
		, m_queryCount{ createInfo.queryCount }
		, m_pipelineStatistics{ getQueryTypes( createInfo.pipelineStatistics ) }
		, m_names( size_t( m_queryCount ), GLuint( GL_INVALID_INDEX ) )
	{
		assert( m_queryType != VK_QUERY_TYPE_PIPELINE_STATISTICS
			|| m_queryCount == m_pipelineStatistics.size() );
		auto context = get( m_device )->getContext();
		glLogCall( context
			, glGenQueries
			, GLsizei( m_names.size() )
			, m_names.data() );
	}

	QueryPool::~QueryPool()
	{
		auto context = get( m_device )->getContext();
		glLogCall( context
			, glDeleteQueries
			, GLsizei( m_names.size() )
			, m_names.data() );
	}

	VkResult QueryPool::getResults( ContextLock const & context
		, uint32_t firstQuery
		, uint32_t queryCount
		, VkDeviceSize stride
		, VkQueryResultFlags flags
		, size_t dataSize
		, void * buffer )const
	{
		assert( firstQuery + queryCount <= m_names.size() );
		auto begin = m_names.begin() + firstQuery;
		auto end = begin + ( m_queryType == VK_QUERY_TYPE_PIPELINE_STATISTICS ? uint32_t( m_pipelineStatistics.size() ) : queryCount );
		auto buf = reinterpret_cast< uint8_t * >( buffer );

		if ( checkFlag( flags, VK_QUERY_RESULT_64_BIT ) )
		{
			stride = stride
				? stride
				: sizeof( GLuint64 );

			for ( auto it = begin; it != end; ++it )
			{
				glLogCall( context
					, glGetQueryObjectui64v
					, *it
					, convertQueryResultFlags( flags )
					, reinterpret_cast< GLuint64 * >( buf ) );
				buf += stride;
			}
		}
		else
		{
			stride = stride
				? stride
				: sizeof( GLuint );

			for ( auto it = begin; it != end; ++it )
			{
				glLogCall( context
					, glGetQueryObjectuiv
					, *it
					, convertQueryResultFlags( flags )
					, reinterpret_cast< GLuint * >( buf ) );
				buf += stride;
			}
		}

		return VK_SUCCESS;
	}
}
