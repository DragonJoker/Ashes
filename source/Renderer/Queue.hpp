/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#pragma once

#include "RendererPrerequisites.hpp"

#include <VkLib/Queue.hpp>

namespace renderer
{
	/**
	*\brief
	*	Classe encapsulant une VkQueue.
	*/
	class Queue
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] queue
		*	La vk::Queue.
		*/
		Queue( vk::Queue const & queue );
		/**
		*\brief
		*	Met en attente des tampons de commandes.
		*\param[in] commandBuffer
		*	Le tampon de commandes.
		*\param[in] fence
		*	Une barrière optionnelle.
		*\return
		*	\p true si tout s'est bien passé.
		*/
		bool submit( CommandBuffer const & commandBuffer
			, Fence const * fence )const;
		/**
		*\brief
		*	Met en attente des tampons de commandes.
		*\param[in] commandBuffer
		*	Le tampon de commandes.
		*\param[in] semaphoreToWait
		*	Le sémaphore à attendre lors de l'éxécution de la file.
		*\param[in] semaphoreStage
		*	L'étape du sémaphore.
		*\param[in] semaphoreToSignal
		*	Le sémaphore à signaler à la fin de l'éxécution de la file.
		*\param[in] fence
		*	Une barrière optionnelle.
		*\return
		*	\p true si tout s'est bien passé.
		*/ 
		bool submit( CommandBuffer const & commandBuffer
			, Semaphore const & semaphoreToWait
			, PipelineStageFlags const & semaphoreStage
			, Semaphore const & semaphoreToSignal
			, Fence const * fence )const;
		/**
		*\brief
		*	Met en attente des tampons de commandes.
		*\param[in] commandBuffers
		*	Les tampons de commandes.
		*\param[in] semaphoresToWait
		*	Les sémaphores à attendre lors de l'éxécution de la file.
		*\param[in] semaphoresStage
		*	Les étapes respectives des sémaphores.
		*\param[in] semaphoresToSignal
		*	Les sémaphores à signaler à la fin de l'éxécution de la file.
		*\param[in] fence
		*	Une barrière optionnelle.
		*\return
		*	\p true si tout s'est bien passé.
		*/ 
		bool submit( CommandBufferCRefArray const & commandBuffers
			, SemaphoreCRefArray const & semaphoresToWait
			, PipelineStageFlagsArray const & semaphoresStage
			, SemaphoreCRefArray const & semaphoresToSignal
			, Fence const * fence )const;
		/**
		*\brief
		*	Présente la file à Vulkan.
		*\return
		*	\p true si tout s'est bien passé.
		*/ 
		bool present( SwapChainCRefArray const & swapChains
			, UInt32Array const & imagesIndex
			, SemaphoreCRefArray const & semaphoresToWait )const;
		/**
		*\brief
		*	Attend que la file soit inactive.
		*\return
		*	\p true si tout s'est bien passé.
		*/
		bool waitIdle()const;
		/**
		*\return
		*	L'index de la famille de la file.
		*/
		inline uint32_t getFamilyIndex()const
		{
			return m_queue.getFamilyIndex();
		}
		/**
		*\return
		*	La vk::Queue.
		*/
		inline vk::Queue const & getQueue()const
		{
			return m_queue;
		}

	private:
		vk::Queue const & m_queue;
	};
}
