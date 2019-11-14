/*
See LICENSE file in root folder
*/
#ifndef ___AshesPP_QueryPool_H___
#define ___AshesPP_QueryPool_H___

#include "ashespp/AshesPPPrerequisites.hpp"

namespace ashes
{
	/**
	*\brief
	*	GPU query pool implementation.
	*/
	class QueryPool
	{
	public:
		/**
		*\brief
		*	Constructor.
		*\param[in] device
		*	The logical device.
		*\param[in] type
		*	The query type.
		*\param[in] count
		*	The number of queries managed by the pool.
		*\param[in] pipelineStatistics
		*	Specifies the counters to be returned in queries.
		*/
		QueryPool( Device const & device
			, VkQueryType type
			, uint32_t count
			, VkQueryPipelineStatisticFlags pipelineStatistics );
		/**
		*\brief
		*	Destructor.
		*/
		~QueryPool();
		/**
		*\brief
		*	Rerieves the status or result of a set of queries.
		*\param[in] firstQuery
		*	The first query index.
		*\param[in] queryCount
		*	The number of queries (starting at \p firstQuery).
		*\param[in] stride
		*	The stride in bytes between results for individual queries, in \p data.
		*\param[in] flags
		*	Specifies how and when results are returned.
		*\param[out] data
		*	Receives the results.
		*/
		void getResults( uint32_t firstQuery
			, uint32_t queryCount
			, uint32_t stride
			, VkQueryResultFlags flags
			, UInt32Array & data )const;
		/**
		*\brief
		*	Rerieves the status or result of a set of queries.
		*\param[in] firstQuery
		*	The first query index.
		*\param[in] queryCount
		*	The number of queries (starting at \p firstQuery).
		*\param[in] stride
		*	The stride in bytes between results for individual queries, in \p data.
		*\param[in] flags
		*	Specifies how and when results are returned.
		*\param[out] data
		*	Receives the results.
		*/
		void getResults( uint32_t firstQuery
			, uint32_t queryCount
			, uint32_t stride
			, VkQueryResultFlags flags
			, UInt64Array & data )const;
		/**
		*\name
		*	Getters.
		*/
		/**@{*/
		inline VkQueryType getType()const
		{
			return m_type;
		}

		inline uint32_t getCount()const
		{
			return m_count;
		}

		inline VkQueryPipelineStatisticFlags const & getPipelineStatistics()const
		{
			return m_pipelineStatistics;
		}
		/**@}*/
		/**
		*\brief
		*	VkQueryPool implicit cast operator.
		*/
		inline operator VkQueryPool const & ()const
		{
			return m_internal;
		}

	protected:
		Device const & m_device;
		VkQueryType m_type;
		uint32_t m_count;
		VkQueryPipelineStatisticFlags m_pipelineStatistics;
		VkQueryPool m_internal{ VK_NULL_HANDLE };
	};
}

#endif
