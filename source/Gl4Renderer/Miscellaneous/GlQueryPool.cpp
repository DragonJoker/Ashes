#include "Miscellaneous/GlQueryPool.hpp"

#include "Core/GlDevice.hpp"
#include "Miscellaneous/GlCallLogger.hpp"

#include "ashesgl4_api.hpp"

namespace ashes::gl4
{
	QueryPool::QueryPool( VkDevice device
		, VkQueryPoolCreateInfo createInfo )
		: m_device{ device }
		, m_flags{ createInfo.flags }
		, m_queryType{ createInfo.queryType }
		, m_queryCount{ createInfo.queryCount }
		, m_pipelineStatistics{ createInfo.pipelineStatistics }
		, m_names( size_t( m_queryCount ), GLuint( GL_INVALID_INDEX ) )
	{
		auto & context = get( m_device )->getContext();
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

	VkResult QueryPool::getResults( uint32_t firstQuery
		, uint32_t queryCount
		, VkDeviceSize stride
		, VkQueryResultFlags flags
		, UInt32Array & data )const
	{
		assert( firstQuery + queryCount <= m_names.size() );
		assert( queryCount == data.size() );
		UInt64Array data64;
		data64.resize( data.size() );
		auto begin = m_names.begin() + firstQuery;
		auto end = begin + queryCount;
		auto * buffer = data64.data();
		auto context = get( m_device )->getContext();

		for ( auto it = begin; it != end; ++it )
		{
			glLogCall( context
				, glGetQueryObjectui64v
				, *it
				, convertQueryResultFlags( flags )
				, buffer );
			++buffer;
		}

		for ( uint32_t i = 0; i < data64.size(); ++i )
		{
			data[i] = uint32_t( data64[i] );
		}

		return VK_SUCCESS;
	}

	VkResult QueryPool::getResults( uint32_t firstQuery
		, uint32_t queryCount
		, VkDeviceSize stride
		, VkQueryResultFlags flags
		, UInt64Array & data )const
	{
		assert( firstQuery + queryCount <= m_names.size() );
		assert( queryCount == data.size() );
		auto begin = m_names.begin() + firstQuery;
		auto end = begin + queryCount;
		auto * buffer = data.data();
		auto context = get( m_device )->getContext();

		for ( auto it = begin; it != end; ++it )
		{
			glLogCall( context
				, glGetQueryObjectui64v
				, *it
				, convertQueryResultFlags( flags )
				, buffer );
			++buffer;
		}

		return VK_SUCCESS;
	}
}
