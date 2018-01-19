#include "GlQueryPool.hpp"

#include <GlDevice.hpp>

namespace gl_renderer
{
	QueryPool::QueryPool( Device const & device
		, renderer::QueryType type
		, uint32_t count
		, renderer::QueryPipelineStatisticFlags pipelineStatistics )
		: renderer::QueryPool{ device, type, count, pipelineStatistics }
		, m_device{ device }
		, m_names( size_t( count ), GLuint( GL_INVALID_INDEX ) )
	{
		glGenQueries( GLsizei( m_names.size() ), m_names.data() );
	}

	QueryPool::~QueryPool()
	{
		glDeleteQueries( GLsizei( m_names.size() ), m_names.data() );
	}

	void QueryPool::getResults( uint32_t firstQuery
		, uint32_t queryCount
		, uint32_t stride
		, renderer::QueryResultFlags flags
		, renderer::UInt32Array & data )const
	{
		assert( firstQuery + queryCount < m_names.size() );
		assert( queryCount == data.size() );
		auto begin = m_names.begin() + firstQuery;
		auto end = begin + queryCount;
		auto * buffer = data.data();

		for ( auto it = begin; it != end; ++it )
		{
			glGetQueryObjectuiv( *it, convert( flags ), buffer );
			++buffer;
		}
	}

	void QueryPool::getResults( uint32_t firstQuery
		, uint32_t queryCount
		, uint32_t stride
		, renderer::QueryResultFlags flags
		, renderer::UInt64Array & data )const
	{
		assert( firstQuery + queryCount < m_names.size() );
		assert( queryCount == data.size() );
		auto begin = m_names.begin() + firstQuery;
		auto end = begin + queryCount;
		auto * buffer = data.data();

		for ( auto it = begin; it != end; ++it )
		{
			glGetQueryObjectui64v( *it, convert( flags ), buffer );
			++buffer;
		}
	}
}
