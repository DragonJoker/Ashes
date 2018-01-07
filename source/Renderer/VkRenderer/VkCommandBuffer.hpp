/*
This file belongs to Renderer.
See LICENSE file in root folder
*/
#pragma once

#include "VkClearValue.hpp"

#include <VkLib/CommandBuffer.hpp>
#include <Renderer/CommandBuffer.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Encapsulation d'un vk::CommandBuffer.
	*/
	class CommandBuffer
		: public renderer::CommandBuffer
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le périphérique logique.
		*\param[in] pool
		*	Le pool de tampons de commandes.
		*\param[in] primary
		*	Dit si le tampon est un tampon de commandes primaire (\p true) ou secondaire (\p false).
		*/
		CommandBuffer( Device const & device
			, CommandPool const & pool
			, bool primary );
		/**
		*\brief
		*	Démarre l'enregistrement du tampon de commandes.
		*\param[in] flags
		*	Les indicateurs de type de charge qui sera affectée au tampon.
		*\return
		*	\p false en cas d'erreur.
		*/
		bool begin( renderer::CommandBufferUsageFlags flags = 0u )const override;
		/**
		*\brief
		*	Démarre l'enregistrement du tampon de commandes en tant que tampon secondaire.
		*\param[in] flags
		*	Les indicateurs de type de charge qui sera affectée au tampon.
		*\param[in] renderPass
		*	La passe de rendu avec laquelle le tampon sera compatible, et dans laquelle il peut s'exécuter.
		*\param[in] subpass
		*	L'indice de la sous-passe au sein de laquelle le tampon de commandes sera exécuté.
		*\param[in] frameBuffer
		*	Le tampon d'images dans lequel le tampon de commandes va effectuer son rendu.
		*\param[in] occlusionQueryEnable
		*	Indique si le tampon de commandes peut être exécuté alors qu'une requête d'occlusion est active sur le tampon principal.
		*\param[in] queryFlags
		*	Les indicateurs de requête d'occlusion pouvant être utilisées par une requête d'occlusion active sur le tampon principal,
		*	lorsque ce tampon est exécuté.
		*\param[in] pipelineStatistics
		*	Indique quelles statistique de pipeline peuvent être comptées par une requête active sur le tampon principal,
		*	lorsque ce tampon est exécuté.
		*\return
		*	\p false en cas d'erreur.
		*/
		bool begin( renderer::CommandBufferUsageFlags flags
			, renderer::RenderPass const & renderPass
			, uint32_t subpass
			, renderer::FrameBuffer const & frameBuffer
			, bool occlusionQueryEnable = false
			, renderer::QueryControlFlags queryFlags = 0u
			, renderer::QueryPipelineStatisticFlags pipelineStatistics = 0u )const override;
		/**
		*\brief
		*	Termine l'enregistrement du tampon de commandes.
		*\return
		*	\p false en cas d'erreur.
		*/
		bool end()const override;
		/**
		*\brief
		*	Réinitialise le tampon de commandes et le met dans un état où il peut à nouveau enregistrer des commandes.
		*\param[in] flags
		*	Les indicateurs contrôlant le comportement de la réinitialisation du tampon.
		*\return
		*	\p false en cas d'erreur.
		*/
		bool reset( renderer::CommandBufferResetFlags flags = 0u )const override;
		/**
		*\brief
		*	Démarre une passe de rendu.
		*\param[in] renderPass
		*	La passe de rendu.
		*\param[in] frameBuffer
		*	Le tampon d'image affecté par le rendu.
		*\param[in] clearValues
		*	Les valeurs de vidage, une par attache de la passe de rendu.
		*\param[in] contents
		*	Indique la manière dont les commandes de la première sous-passe sont fournies.
		*/
		void beginRenderPass( renderer::RenderPass const & renderPass
			, renderer::FrameBuffer const & frameBuffer
			, renderer::ClearValueArray const & clearValues
			, renderer::SubpassContents contents )const override;
		/**
		*\brief
		*	Passe à la sous-passe suivante.
		*\param[in] contents
		*	Indique la manière dont les commandes de la sous-passe suivante sont fournies.
		*/
		void nextSubpass( renderer::SubpassContents contents )const override;
		/**
		*\brief
		*	Termine une passe de rendu.
		*/
		void endRenderPass()const override;
		/**
		*\brief
		*	Execute des tampons de commande secondaires.
		*\param[in] commands
		*	Les tampons de commandes.
		*/
		void executeCommands( renderer::CommandBufferCRefArray const & commands )const override;
		/**
		*\brief
		*	Vide l'image avec la couleur de vidage.
		*\param[in] image
		*	L'image à vider.
		*\param[in] colour
		*	La couleur de vidage.
		*/
		void clear( renderer::Texture const & image
			, renderer::RgbaColour const & colour )const override;
		/**
		*\brief
		*	Met en place une barrière de transition d'état de tampon.
		*\param[in] after
		*	Les étapes devant être terminées avant l'exécution de la barrière.
		*\param[in] before
		*	Les étapes pouvant être commencées après l'exécution de la barrière.
		*\param[in] transitionBarrier
		*	La description de la transition.
		*/
		void memoryBarrier( renderer::PipelineStageFlags after
			, renderer::PipelineStageFlags before
			, renderer::BufferMemoryBarrier const & transitionBarrier )const override;
		/**
		*\brief
		*	Met en place une barrière de transition de layout d'image.
		*\param[in] after
		*	Les étapes devant être terminées avant l'exécution de la barrière.
		*\param[in] before
		*	Les étapes pouvant être commencées après l'exécution de la barrière.
		*\param[in] transitionBarrier
		*	La description de la transition.
		*/
		void memoryBarrier( renderer::PipelineStageFlags after
			, renderer::PipelineStageFlags before
			, renderer::ImageMemoryBarrier const & transitionBarrier )const override;
		/**
		*\brief
		*	Active un pipeline: shaders, tests, états, ...
		*\param[in] pipeline
		*	Le pipeline à activer.
		*\param[in] bindingPoint
		*	Le point d'attache du pipeline.
		*/
		void bindPipeline( renderer::Pipeline const & pipeline
			, renderer::PipelineBindPoint bindingPoint = renderer::PipelineBindPoint::eGraphics )const override;
		/**
		*\brief
		*	Active des tampons de géométrie.
		*\param[in] geometryBuffers
		*	Les tampons de géométrie.
		*/
		void bindGeometryBuffers( renderer::GeometryBuffers const & geometryBuffers )const override;
		/**
		*\brief
		*	Active un descriptor set.
		*\param[in] descriptorSet
		*	Le descriptor set.
		*\param[in] layout
		*	Le layout de pipeline.
		*\param[in] bindingPoint
		*	Le point d'attache du set.
		*/
		void bindDescriptorSet( renderer::DescriptorSet const & descriptorSet
			, renderer::PipelineLayout const & layout
			, renderer::PipelineBindPoint bindingPoint = renderer::PipelineBindPoint::eGraphics )const override;
		/**
		*\brief
		*	Définit le viewport du pipeline.
		*\remarks
		*	Cette action n'est faisable que si le viewport est configuré comme dynamique.
		*\param[in] viewport
		*	Le viewport.
		*/
		void setViewport( renderer::Viewport const & viewport )const override;
		/**
		*\brief
		*	Définit le ciseau du pipeline.
		*\remarks
		*	Cette action n'est faisable que si le ciseau est configuré comme dynamique.
		*\param[in] scissor
		*	Le ciseau.
		*/
		void setScissor( renderer::Scissor const & scissor )const override;
		/**
		*\brief
		*	Dessine des sommets.
		*\param[in] vtxCount
		*	Nombre de sommets.
		*\param[in] instCount
		*	Nombre d'instances.
		*\param[in] firstVertex
		*	Index du premier sommet.
		*\param[in] firstInstance
		*	Index de la première instance.
		*/
		void draw( uint32_t vtxCount
			, uint32_t instCount
			, uint32_t firstVertex
			, uint32_t firstInstance )const override;
		/**
		*\brief
		*	Dessine des sommets.
		*\param[in] indexCount
		*	Nombre d'indices.
		*\param[in] instCount
		*	Nombre d'instances.
		*\param[in] firstIndex
		*	Index du premier indice.
		*\param[in] vertexOffset
		*	La valeur ajoutée à l'indice du sommet avant d'indexer le tampon de sommets.
		*\param[in] firstInstance
		*	Index de la première instance.
		*/
		void drawIndexed( uint32_t indexCount
			, uint32_t instCount
			, uint32_t firstIndex
			, uint32_t vertexOffset
			, uint32_t firstInstance )const override;
		/**
		*\brief
		*	Copie les données d'un tampon vers un autre tampon.
		*\param[in] src
		*	Le tampon source.
		*\param[in] dst
		*	Le tampon destination.
		*\param[in] size
		*	La taille des données à copier.
		*/
		void copyBuffer( renderer::BufferBase const & src
			, renderer::BufferBase const & dst
			, uint32_t size
			, uint32_t offset = 0 )const override;
		/**
		*\brief
		*	Copie les données d'un tampon vers un autre tampon.
		*\param[in] src
		*	Le tampon source.
		*\param[in] dst
		*	Le tampon destination.
		*\param[in] size
		*	La taille des données à copier.
		*/
		void copyBuffer( renderer::BufferBase const & src
			, renderer::VertexBufferBase const & dst
			, uint32_t size
			, uint32_t offset = 0 )const override;
		/**
		*\brief
		*	Copie les données d'un tampon vers un autre tampon.
		*\param[in] src
		*	Le tampon source.
		*\param[in] dst
		*	Le tampon destination.
		*\param[in] size
		*	La taille des données à copier.
		*/
		void copyBuffer( renderer::VertexBufferBase const & src
			, renderer::BufferBase const & dst
			, uint32_t size
			, uint32_t offset = 0 )const override;
		/**
		*\brief
		*	Copie les données d'un tampon vers un autre tampon.
		*\param[in] src
		*	Le tampon source.
		*\param[in] dst
		*	Le tampon destination.
		*\param[in] size
		*	La taille des données à copier.
		*/
		void copyBuffer( renderer::VertexBufferBase const & src
			, renderer::VertexBufferBase const & dst
			, uint32_t size
			, uint32_t offset = 0 )const override;
		/**
		*\brief
		*	Copie les données d'un tampon vers un autre tampon.
		*\param[in] src
		*	Le tampon source.
		*\param[in] dst
		*	Le tampon destination.
		*\param[in] size
		*	La taille des données à copier.
		*/
		void copyBuffer( renderer::BufferBase const & src
			, renderer::UniformBufferBase const & dst
			, uint32_t size
			, uint32_t offset = 0 )const override;
		/**
		*\brief
		*	Copie les données d'un tampon vers un autre tampon.
		*\param[in] src
		*	Le tampon source.
		*\param[in] dst
		*	Le tampon destination.
		*\param[in] size
		*	La taille des données à copier.
		*/
		void copyBuffer( renderer::UniformBufferBase const & src
			, renderer::BufferBase const & dst
			, uint32_t size
			, uint32_t offset = 0 )const override;
		/**
		*\brief
		*	Copie les données d'un tampon vers un autre tampon.
		*\param[in] src
		*	Le tampon source.
		*\param[in] dst
		*	Le tampon destination.
		*\param[in] size
		*	La taille des données à copier.
		*/
		void copyBuffer( renderer::UniformBufferBase const & src
			, renderer::UniformBufferBase const & dst
			, uint32_t size
			, uint32_t offset = 0 )const override;
		/**
		*\brief
		*	Copie les données d'un tampon vers une image.
		*\param[in] src
		*	Le tampon source.
		*\param[in] dst
		*	L'image destination.
		*/
		void copyImage( renderer::BufferBase const & src
			, renderer::Texture const & dst )const override;
		/**
		*\brief
		*	Copie les données d'un tampon vers une image.
		*\param[in] src
		*	Le tampon source.
		*\param[in] dst
		*	L'image destination.
		*/
		void copyImage( renderer::StagingBuffer const & src
			, renderer::Texture const & dst )const override;
		/**
		*\~french
		*\return
		*	Le pipeline actuellement actif.
		*\~english
		*\return
		*	The currently active pipeline.
		*/
		inline Pipeline const & getCurrentPipeline()const
		{
			assert( m_currentPipeline && "No pipeline bound." );
			return *m_currentPipeline;
		}
		/**
		*\~french
		*\return
		*	Dit si un pipeline est actif.
		*\~english
		*\return
		*	Tells if a pipeline is active.
		*/
		inline bool isPipelineBound()const
		{
			return m_currentPipeline != nullptr;
		}
		/**
		*\~french
		*\brief
		*	Conversion implicite vers VkCommandBuffer.
		*\~english
		*\brief
		*	VkCommandBuffer implicit cast operator.
		*/
		inline operator VkCommandBuffer const &( )const
		{
			return m_commandBuffer;
		}

	private:
		Device const & m_device;
		CommandPool const & m_pool;
		VkCommandBuffer m_commandBuffer{};
		mutable Pipeline const * m_currentPipeline{ nullptr };
		mutable VkCommandBufferInheritanceInfo m_inheritanceInfo;
	};
}
