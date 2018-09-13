#include "Miscellaneous/GlQueryPool.hpp"

#include "Core/GlDevice.hpp"

namespace gl_renderer
{
	QueryPool::QueryPool( Device const & device
		, ashes::QueryType type
		, uint32_t count
		, ashes::QueryPipelineStatisticFlags pipelineStatistics )
		: ashes::QueryPool{ device, type, count, pipelineStatistics }
		, m_device{ device }
		, m_names( size_t( count ), GLuint( GL_INVALID_INDEX ) )
	{
		auto context = m_device.getContext();
		glLogCall( context
			, glGenQueries
			, GLsizei( m_names.size() )
			, m_names.data() );
	}

	QueryPool::~QueryPool()
	{
		auto context = m_device.getContext();
		glLogCall( context
			, glDeleteQueries
			, GLsizei( m_names.size() )
			, m_names.data() );
	}

	void QueryPool::getResults( uint32_t firstQuery
		, uint32_t queryCount
		, uint32_t stride
		, ashes::QueryResultFlags flags
		, ashes::UInt32Array & data )const
	{
		assert( firstQuery + queryCount <= m_names.size() );
		assert( queryCount == data.size() );
		auto context = m_device.getContext();
		ashes::UInt64Array data64;
		data64.resize( data.size() );
		auto begin = m_names.begin() + firstQuery;
		auto end = begin + queryCount;
		auto * buffer = data64.data();

		for ( auto it = begin; it != end; ++it )
		{
			glLogCall( context
				, glGetQueryObjectui64v
				, *it
				, convert( flags )
				, buffer );
			++buffer;
		}

		for ( uint32_t i = 0; i < data64.size(); ++i )
		{
			data[i] = uint32_t( data64[i] );
		}
	}

	void QueryPool::getResults( uint32_t firstQuery
		, uint32_t queryCount
		, uint32_t stride
		, ashes::QueryResultFlags flags
		, ashes::UInt64Array & data )const
	{
		assert( firstQuery + queryCount <= m_names.size() );
		assert( queryCount == data.size() );
		auto context = m_device.getContext();
		auto begin = m_names.begin() + firstQuery;
		auto end = begin + queryCount;
		auto * buffer = data.data();

		for ( auto it = begin; it != end; ++it )
		{
			glLogCall( context
				, glGetQueryObjectui64v
				, *it
				, convert( flags )
				, buffer );
			++buffer;
		}
	}
}
