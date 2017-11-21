/**
*\file
*	RenderingResources.h
*\author
*	Sylvain Doremus
*/
#ifndef ___Renderer_RenderingResources_HPP___
#define ___Renderer_RenderingResources_HPP___
#pragma once

#include "CommandBuffer.hpp"

#include <VkLib/Fence.hpp>
#include <VkLib/FrameBuffer.hpp>
#include <VkLib/Semaphore.hpp>

namespace renderer
{
	/**
	*\brief
	*	Classe regroupant les ressources de rendu nécessaires au dessin d'une image.
	*/
	class RenderingResources
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le LogicalDevice parent.
		*\param[in] familyIndex
		*	L'index du type de file.
		*/
		RenderingResources( vk::LogicalDevice const & device
			, vk::CommandPool const & commandPool );
		/**
		*\brief
		*	Attend que le tampon de commandes soit prêt à l'enregistrement.
		*\param[in] timeout
		*	Le temps à attendre pour le signalement.
		*\return
		*	\p true si l'attente n'est pas sortie en timeout.
		*/
		bool waitRecord( uint32_t timeout );
		/**
		*\brief
		*	Définit le tampon d'images.
		*\param[in] frameBuffer
		*	Le nouveau tampon d'images.
		*/
		inline void setFrameBuffer( vk::FrameBufferPtr && frameBuffer )
		{
			m_frameBuffer = std::move( frameBuffer );
		}
		/**
		*\return
		*	Le tampon d'images.
		*/
		inline auto const & getFrameBuffer()const
		{
			return *m_frameBuffer;
		}
		/**
		*\return
		*	Le sémaphore d'attente que l'image soit disponible.
		*/
		inline auto const & getImageAvailableSemaphore()const
		{
			return *m_imageAvailableSemaphore;
		}
		/**
		*\return
		*	Le sémaphore d'attente que le rendu soit terminé.
		*/
		inline auto const & getRenderingFinishedSemaphore()const
		{
			return *m_finishedRenderingSemaphore;
		}
		/**
		*\return
		*	Le tampon de commandes.
		*/
		inline CommandBuffer const & getCommandBuffer()const
		{
			return m_commandBuffer;
		}
		/**
		*\return
		*	La barrière.
		*/
		inline auto const & getFence()const
		{
			return m_fence;
		}
		/**
		*\return
		*	La périphérique logique.
		*/
		inline auto const & getDevice()const
		{
			return m_device;
		}
		/**
		*\return
		*	Le tampon de transfert.
		*/
		inline StagingBuffer const & getStagingBuffer()const
		{
			return *m_stagingBuffer;
		}

	private:
		vk::LogicalDevice const & m_device;
		vk::FrameBufferPtr m_frameBuffer;
		vk::SemaphorePtr m_imageAvailableSemaphore;
		vk::SemaphorePtr m_finishedRenderingSemaphore;
		vk::Fence m_fence;
		CommandBuffer m_commandBuffer;
		StagingBufferPtr m_stagingBuffer;
	};
}

#endif
