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

#include "Fence.hpp"
#include "FrameBuffer.hpp"
#include "Semaphore.hpp"

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
		*	Le Device parent.
		*/
		RenderingResources( Device const & device );
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
		*	Définit le tampon de fenêtre.
		*\param[in] backBuffer
		*	Le nouveau tampon de fenêtre.
		*/
		inline void setBackBuffer( uint32_t backBuffer )
		{
			m_backBuffer = backBuffer;
		}
		/**
		*\return
		*	Le tampon de fenêtre.
		*/
		inline uint32_t getBackBuffer()const
		{
			return m_backBuffer;
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

	private:
		Device const & m_device;
		SemaphorePtr m_imageAvailableSemaphore;
		SemaphorePtr m_finishedRenderingSemaphore;
		Fence m_fence;
		CommandBuffer m_commandBuffer;
		uint32_t m_backBuffer{ 0u };
	};
}

#endif
