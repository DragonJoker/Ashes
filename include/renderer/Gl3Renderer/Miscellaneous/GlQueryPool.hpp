/*
See LICENSE file in root folder
*/
#ifndef ___GlRenderer_QueryPool_H___
#define ___GlRenderer_QueryPool_H___

#include "Gl3Renderer/GlRendererPrerequisites.hpp"

#include <Ashes/Miscellaneous/QueryPool.hpp>

namespace gl_renderer
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
		: public ashes::QueryPool
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
		*	The number of queries managed by the pool.
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
		*	Le nombre de requêtes gérées par le pool.
		*\param[in] pipelineStatistics
		*	Définit les compteurs que les requêtes retournent.
		*/
		QueryPool( Device const & device
			, VkQueryType type
			, uint32_t count
			, ashes::QueryPipelineStatisticFlags pipelineStatistics );
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
		*\copydoc	ashes::QueryPool:getResults
		*/
		void getResults( uint32_t firstQuery
			, uint32_t queryCount
			, uint32_t stride
			, VkQueryResultFlags flags
			, ashes::UInt32Array & data )const override;
		/**
		*\copydoc	ashes::QueryPool:getResults
		*/
		void getResults( uint32_t firstQuery
			, uint32_t queryCount
			, uint32_t stride
			, VkQueryResultFlags flags
			, ashes::UInt64Array & data )const override;
		/**
		*\return
		*	Le début du tableau de requêtes.
		*/
		inline auto begin()const
		{
			return m_names.begin();
		}
		/**
		*\return
		*	La fin du tableau de requêtes.
		*/
		inline auto end()const
		{
			return m_names.end();
		}

	protected:
		Device const & m_device;
		std::vector< GLuint > m_names;
	};
}

#endif
