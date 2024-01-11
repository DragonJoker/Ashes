/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include <ashespp/Command/CommandBuffer.hpp>
#include <ashespp/Sync/Fence.hpp>
#include <ashespp/Sync/Semaphore.hpp>

namespace utils
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
		RenderingResources( ashes::Device const & device
			, ashes::SwapChain const & swapChain
			, ashes::CommandPool const & commandPool );
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
		bool waitRecord( uint64_t timeout )const;
		/**
		*\~english
		*\brief
		*	Presents the swapchain image to the given queue.
		*\param[in] queue
		*	The queue used for presentation.
		*\return
		*	\p VK_SUCCESS if everything's fine.
		*\~french
		*\brief
		*	Présente l'image de la swapchain à la file donnée.
		*\param[in] queue
		*	La file de présentation.
		*\return
		*	\p VK_SUCCESS si tout va bien.
		*/
		VkResult present( ashes::Queue const & queue )const;

		void setImageIndex( uint32_t imageIndex )noexcept
		{
			m_imageIndex = imageIndex;
		}

		uint32_t getImageIndex()const noexcept
		{
			return m_imageIndex;
		}

		ashes::Semaphore const & getImageAvailableSemaphore()const noexcept
		{
			return *m_imageAvailableSemaphore;
		}

		ashes::Semaphore const & getRenderingFinishedSemaphore()const noexcept
		{
			return *m_finishedRenderingSemaphore;
		}

		ashes::CommandBuffer const & getCommandBuffer()const noexcept
		{
			return *m_commandBuffer;
		}

		ashes::Fence const & getFence()const noexcept
		{
			return *m_fence;
		}

		ashes::Device const & getDevice()const noexcept
		{
			return m_device;
		}

		ashes::SwapChain const & getSwapChain()const noexcept
		{
			return m_swapChain;
		}

	private:
		ashes::Device const & m_device;
		ashes::SwapChain const & m_swapChain;
		ashes::SemaphorePtr m_imageAvailableSemaphore;
		ashes::SemaphorePtr m_finishedRenderingSemaphore;
		ashes::FencePtr m_fence;
		ashes::CommandBufferPtr m_commandBuffer;
		uint32_t m_imageIndex{ 0u };
	};

	using RenderingResourcesPtr = std::unique_ptr< RenderingResources >;
	using RenderingResourcesArray = std::vector< RenderingResourcesPtr >;
}

