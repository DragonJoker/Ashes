/*
This file belongs to VkLib.
See LICENSE file in root folder
*/
#pragma once

#include "RendererPrerequisites.hpp"

#include <VkLib/PrimaryCommandBuffer.hpp>

namespace renderer
{
	/**
	*\brief
	*	Encapsulation d'un vk::CommandBuffer.
	*/
	class CommandBuffer
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] resources
		*	Les ressources de rendu.
		*/
		CommandBuffer( RenderingResources const & resources
			, vk::CommandPool const & pool );
		/**
		*\brief
		*	Démarre l'enregistrement du tampon de commandes.
		*\param[in] flags
		*	Les indicateurs de type de charge qui sera affectée au tampon.
		*\return
		*	\p false en cas d'erreur.
		*/
		bool begin( CommandBufferUsageFlags flags = 0u )const;
		/**
		*\brief
		*	Termine l'enregistrement du tampon de commandes.
		*\return
		*	\p false en cas d'erreur.
		*/
		bool end()const;
		/**
		*\brief
		*	Réinitialise le tampon de commandes et le met dans un état où il peut à nouveau enregistrer des commandes.
		*\param[in] flags
		*	Les indicateurs contrôlant le comportement de la réinitialisation du tampon.
		*\return
		*	\p false en cas d'erreur.
		*/
		bool reset( CommandBufferResetFlags flags = 0u )const;
		/**
		*\brief
		*	Vide l'image avec la couleur de vidage.
		*\param[in] image
		*	L'image à vider.
		*\param[in] colour
		*	La couleur de vidage.
		*/
		void clear( Texture const & image
			, RgbaColour const & colour )const;
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
		void memoryBarrier( PipelineStageFlags after
			, PipelineStageFlags before
			, BufferMemoryBarrier const & transitionBarrier )const;
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
		void memoryBarrier( PipelineStageFlags after
			, PipelineStageFlags before
			, ImageMemoryBarrier const & transitionBarrier )const;
		/**
		*\brief
		*	Active un pipeline: shaders, tests, états, ...
		*\param[in] pipeline
		*	Le pipeline à activer.
		*\param[in] bindingPoint
		*	Le point d'attache du pipeline.
		*/
		void bindPipeline( Pipeline const & pipeline
			, PipelineBindPoint bindingPoint = PipelineBindPoint::eGraphics )const;
		/**
		*\brief
		*	Active un tampon de sommets.
		*\param[in] vertexBuffer
		*	Le tampon de sommets.
		*\param[in] offset
		*	L'offset du premier sommet dans le tampon.
		*/
		template< typename T >
		void bindVertexBuffer( VertexBuffer< T > const & vertexBuffer
			, uint64_t offset )const;
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
		void bindDescriptorSet( DescriptorSet const & descriptorSet
			, PipelineLayout const & layout
			, PipelineBindPoint bindingPoint = PipelineBindPoint::eGraphics )const;
		/**
		*\brief
		*	Définit le viewport du pipeline.
		*\remarks
		*	Cette action n'est faisable que si le viewport est configuré comme dynamique.
		*\param[in] viewport
		*	Le viewport.
		*/
		void setViewport( Viewport const & viewport )const;
		/**
		*\brief
		*	Définit le ciseau du pipeline.
		*\remarks
		*	Cette action n'est faisable que si le ciseau est configuré comme dynamique.
		*\param[in] scissor
		*	Le ciseau.
		*/
		void setScissor( Scissor const & scissor )const;
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
			, uint32_t firstInstance )const;
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
		template< typename T >
		void copyBuffer( Buffer< T > const & src
			, Buffer< T > const & dst
			, uint32_t size
			, uint32_t offset = 0 )const;
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
		template< typename T >
		void copyBuffer( Buffer< T > const & src
			, VertexBuffer< T > const & dst
			, uint32_t size
			, uint32_t offset = 0 )const;
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
		template< typename T >
		void copyBuffer( VertexBuffer< T > const & src
			, Buffer< T > const & dst
			, uint32_t size
			, uint32_t offset = 0 )const;
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
		template< typename T >
		void copyBuffer( VertexBuffer< T > const & src
			, VertexBuffer< T > const & dst
			, uint32_t size
			, uint32_t offset = 0 )const;
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
		template< typename T >
		void copyBuffer( Buffer< T > const & src
			, UniformBuffer< T > const & dst
			, uint32_t size
			, uint32_t offset = 0 )const;
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
		template< typename T >
		void copyBuffer( UniformBuffer< T > const & src
			, Buffer< T > const & dst
			, uint32_t size
			, uint32_t offset = 0 )const;
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
		template< typename T >
		void copyBuffer( UniformBuffer< T > const & src
			, UniformBuffer< T > const & dst
			, uint32_t size
			, uint32_t offset = 0 )const;
		/**
		*\brief
		*	Copie les données d'un tampon vers une image.
		*\param[in] src
		*	Le tampon source.
		*\param[in] dst
		*	L'image destination.
		*/
		template< typename T >
		void copyImage( Buffer< T > const & src
			, Texture const & dst )const;
		/**
		*\return
		*	Le tampon de commandes vulkan.
		*/
		vk::PrimaryCommandBuffer getCommandBuffer()const
		{
			return m_commandBuffer;
		}

	private:
		RenderingResources const & m_resources;
		vk::PrimaryCommandBuffer m_commandBuffer;
	};
}

#include "CommandBuffer.inl"
