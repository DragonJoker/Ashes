/*
See LICENSE file in root folder
*/
#ifndef ___GlRenderer_QueryPool_H___
#define ___GlRenderer_QueryPool_H___

#include "renderer/Gl4Renderer/GlRendererPrerequisites.hpp"

namespace ashes::gl4
{
	/**
	*\~english
	*\brief
	*	GPU query pool implementation.
	*\~french
	*\brief
	*	Implémentation d'un pool de requêtes GPU.
	*/
	class QueryPool
	{
	public:
		/**
		*\~english
		*\brief
		*	Constructor.
		*\param[in] device
		*	The logical device.
		*\param[in] type
		*	The query type.
		*\param[in] count
		*	The number of queries managed by the get( pool )->
		*\param[in] pipelineStatistics
		*	Specifies the counters to be returned in queries.
		*\~french
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le périphérique logique.
		*\param[in] type
		*	Le type de requête.
		*\param[in] count
		*	Le nombre de requêtes gérées par le get( pool )->
		*\param[in] pipelineStatistics
		*	Définit les compteurs que les requêtes retournent.
		*/
		QueryPool( VkDevice device
			, VkQueryPoolCreateInfo createInfo );
		/**
		*\~english
		*\brief
		*	Destructor.
		*\~french
		*\brief
		*	Destructeur.
		*/
		~QueryPool();
		/**
		*\copydoc	QueryPool:getResults
		*/
		VkResult getResults( uint32_t firstQuery
			, uint32_t queryCount
			, VkDeviceSize stride
			, VkQueryResultFlags flags
			, UInt32Array & data )const;
		/**
		*\copydoc	QueryPool:getResults
		*/
		VkResult getResults( uint32_t firstQuery
			, uint32_t queryCount
			, VkDeviceSize stride
			, VkQueryResultFlags flags
			, UInt64Array & data )const;

		inline auto begin()const
		{
			return m_names.begin();
		}

		inline auto end()const
		{
			return m_names.end();
		}

		inline auto getType()const
		{
			return m_queryType;
		}

	protected:
		VkDevice m_device;
		VkQueryPoolCreateFlags m_flags;
		VkQueryType m_queryType;
		uint32_t m_queryCount;
		VkQueryPipelineStatisticFlags m_pipelineStatistics;
		std::vector< GLuint > m_names;
	};
}

#endif
