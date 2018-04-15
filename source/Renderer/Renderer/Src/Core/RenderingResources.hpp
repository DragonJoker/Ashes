/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#ifndef ___Renderer_RenderingResources_HPP___
#define ___Renderer_RenderingResources_HPP___
#pragma once

#include "Command/CommandBuffer.hpp"
#include "Sync/Fence.hpp"
#include "Sync/Semaphore.hpp"

namespace renderer
{
	/**
	*\~english
	*\brief
	*	Holds the resources necessary to work on a swapchain's surface.
	*\~french
	*\brief
	*	Regroupe les ressources de nécessaires pour travailler sur une surface de la swapchain.
	*/
	class RenderingResources
	{
	public:
		/**
		*\~english
		*\brief
		*	Constructor.
		*\param[in] device
		*	The parent Device.
		*\~french
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le Device parent.
		*/
		RenderingResources( Device const & device );
		/**
		*\~english
		*\brief
		*	Destructor.
		*\~french
		*\brief
		*	Destructeur.
		*/
		virtual ~RenderingResources() = default;
		/**
		*\~english
		*\brief
		*	Waits for the command buffer to be ready to record.
		*\param[in] timeout
		*	The waiting timeout.
		*\return
		*	\p true if the waiting didn't end on a timeout.
		*\~french
		*\brief
		*	Attend que le tampon de commandes soit prêt à l'enregistrement.
		*\param[in] timeout
		*	Le temps à attendre pour le signalement.
		*\return
		*	\p true si l'attente n'est pas sortie en timeout.
		*/
		bool waitRecord( uint64_t timeout );
		/**
		*\~english
		*\brief
		*	Sets the back buffer index.
		*\param[in] backBuffer
		*	The new value.
		*\~french
		*\brief
		*	Définit l'indice du tampon de fenêtre.
		*\param[in] backBuffer
		*	La nouvelle valeur.
		*/
		inline void setBackBuffer( uint32_t backBuffer )
		{
			m_backBuffer = backBuffer;
		}
		/**
		*\~english
		*\return
		*	The back buffer index.
		*\~french
		*\return
		*	L'indice du tampon de fenêtre.
		*/
		inline uint32_t getBackBuffer()const
		{
			return m_backBuffer;
		}
		/**
		*\~english
		*\return
		*	The semaphore used to wait for the availability of the surface's image.
		*\~french
		*\return
		*	Le sémaphore d'attente que l'image de la surface soit disponible.
		*/
		inline Semaphore const & getImageAvailableSemaphore()const
		{
			return *m_imageAvailableSemaphore;
		}
		/**
		*\~english
		*\return
		*	The semaphore used to wait for the presentation end.
		*\~french
		*\return
		*	Le sémaphore d'attente que la présentation soit terminée.
		*/
		inline Semaphore const & getRenderingFinishedSemaphore()const
		{
			return *m_finishedRenderingSemaphore;
		}
		/**
		*\~english
		*\return
		*	The command buffer.
		*\~french
		*\return
		*	Le tampon de commandes.
		*/
		inline CommandBuffer const & getCommandBuffer()const
		{
			return *m_commandBuffer;
		}
		/**
		*\~english
		*\return
		*	The fence.
		*\~french
		*\return
		*	La barrière.
		*/
		inline Fence const & getFence()const
		{
			return *m_fence;
		}
		/**
		*\~english
		*\return
		*	The parent device.
		*\~french
		*\return
		*	Le périphérique parent.
		*/
		inline Device const & getDevice()const
		{
			return m_device;
		}

	protected:
		Device const & m_device;
		SemaphorePtr m_imageAvailableSemaphore;
		SemaphorePtr m_finishedRenderingSemaphore;
		FencePtr m_fence;
		CommandBufferPtr m_commandBuffer;
		uint32_t m_backBuffer{ 0u };
	};
}

#endif
