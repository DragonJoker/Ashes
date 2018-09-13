/*
See LICENSE file in root folder
*/
#ifndef ___D3D11Renderer_QueryPool_H___
#define ___D3D11Renderer_QueryPool_H___

#include "D3D11RendererPrerequisites.hpp"

#include <Miscellaneous/QueryPool.hpp>

namespace d3d11_renderer
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
		: public renderer::QueryPool
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
			, renderer::QueryType type
			, uint32_t count
			, renderer::QueryPipelineStatisticFlags pipelineStatistics );
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
		*\copydoc	renderer::QueryPool:getResults
		*/
		void getResults( uint32_t firstQuery
			, uint32_t queryCount
			, uint32_t stride
			, renderer::QueryResultFlags flags
			, renderer::UInt32Array & data )const override;
		/**
		*\copydoc	renderer::QueryPool:getResults
		*/
		void getResults( uint32_t firstQuery
			, uint32_t queryCount
			, uint32_t stride
			, renderer::QueryResultFlags flags
			, renderer::UInt64Array & data )const override;
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
		Device const & m_device;
		std::vector< ID3D11Query * > m_queries;
		mutable renderer::ByteArray m_data;
		std::function< uint32_t( uint32_t ) > getUint32;
		std::function< uint64_t( uint32_t ) > getUint64;
	};
}

#endif
