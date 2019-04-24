/*
See LICENSE file in root folder
*/
#ifndef ___D3D11Renderer_QueryPool_H___
#define ___D3D11Renderer_QueryPool_H___

#include "D3D11Renderer/D3D11RendererPrerequisites.hpp"

#include <Ashes/Miscellaneous/QueryPool.hpp>

namespace ashes::d3d11
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
		QueryPool( VkDevice device
			, VkQueryType type
			, uint32_t count
			, VkQueryPipelineStatisticFlags pipelineStatistics );
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
			, ashes::UInt32Array & data )const;
		/**
		*\copydoc	ashes::QueryPool:getResults
		*/
		void getResults( uint32_t firstQuery
			, uint32_t queryCount
			, uint32_t stride
			, VkQueryResultFlags flags
			, ashes::UInt64Array & data )const;
		/**
		*\return
		*	Le début du tableau de requêtes.
		*/
		inline auto begin()const
		{
			return m_queries.begin();
		}
		/**
		*\return
		*	La fin du tableau de requêtes.
		*/
		inline auto end()const
		{
			return m_queries.end();
		}

	private:

	private:
		VkDevice m_device;
		std::vector< ID3D11Query * > m_queries;
		mutable ashes::ByteArray m_data;
		std::function< uint32_t( uint32_t ) > getUint32;
		std::function< uint64_t( uint32_t ) > getUint64;
	};
}

#endif
