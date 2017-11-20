/**
*\file
*	RenderingResources.h
*\author
*	Sylvain Doremus
*/
#ifndef ___RenderLib_RenderingResources_HPP___
#define ___RenderLib_RenderingResources_HPP___
#pragma once

#include <VkLib/FrameBuffer.hpp>
#include <VkLib/Fence.hpp>
#include <VkLib/PrimaryCommandBuffer.hpp>
#include <VkLib/Semaphore.hpp>

namespace render
{
	/**
	*@~french
	*@brief
	*	Classe regroupant les ressources de rendu nécessaires au dessin d'une image.
	*@~english
	*@brief
	*	Class used to group the image rendering necessary informations.
	*/
	class RenderingResources
	{
	public:
		/**
		*@~french
		*@brief
		*	Constructeur.
		*@param[in] device
		*	Le LogicalDevice parent.
		*@param[in] familyIndex
		*	L'index du type de file.
		*@~english
		*@brief
		*	Constructor.
		*@param[in] device
		*	The parent LogicalDevice.
		*@param[in] familyIndex
		*	The queue family type index.
		*/
		RenderingResources( vk::LogicalDevice const & device
			, vk::CommandPool const & commandPool );
		/**
		*@~french
		*@brief
		*	Attend que le tampon de commandes soit prêt à l'enregistrement.
		*@param[in] timeout
		*	Le temps à attendre pour le signalement.
		*@return
		*	\p true si l'attente n'est pas sortie en timeout.
		*@~english
		*@brief
		*	Waits for the command buffer to be ready to record.
		*@param[in] timeout
		*	The waiting timeout.
		*@return
		*	\p true if the wait didn't timeout.
		*/
		bool waitRecord( uint32_t timeout );
		/**
		*@~french
		*@brief
		*	Copie les données d'une image dans une texture, en passant par le tampon de transfert.
		*@param[in] data
		*	Les données à copier.
		*@param[in] width, height
		*	Les dimensions de l'image.
		*@param[out] texture
		*	La texture de destination.
		*@~english
		*@brief
		*	Copies image data to given texture, using the staging buffer.
		*@param[in] data
		*	The data to copy.
		*@param[in] width, height
		*	The image dimensions.
		*@param[out] texture
		*	The dextination texture.
		*/
		void copyTextureData( vk::ByteArray const & data
			, vk::Image & texture )const;
		/**
		*@~french
		*@brief
		*	Définit le tampon d'images.
		*@param[in] frameBuffer
		*	Le nouveau tampon d'images.
		*@~english
		*@brief
		*	Sets the current frame buffer.
		*@param[in] frameBuffer
		*	The new frame buffer.
		*/
		inline void setFrameBuffer( vk::FrameBufferPtr && frameBuffer )
		{
			m_frameBuffer = std::move( frameBuffer );
		}
		/**
		*@~french
		*@return
		*	Le tampon d'images.
		*@~english
		*@return
		*	The frame buffer.
		*/
		inline auto const & getFrameBuffer()const
		{
			return *m_frameBuffer;
		}
		/**
		*@~french
		*@return
		*	Le sémaphore d'attente que l'image soit disponible.
		*@~english
		*@return
		*	The semaphore used to wait for an image to be available.
		*/
		inline auto const & getImageAvailableSemaphore()const
		{
			return *m_imageAvailableSemaphore;
		}
		/**
		*@~french
		*@return
		*	Le sémaphore d'attente que le rendu soit terminé.
		*@~english
		*@return
		*	The semaphore used to wait for the render to be finished.
		*/
		inline auto const & getRenderingFinishedSemaphore()const
		{
			return *m_finishedRenderingSemaphore;
		}
		/**
		*@~french
		*@return
		*	Le tampon de commandes.
		*@~english
		*@return
		*	The command buffer.
		*/
		inline auto const & getCommandBuffer()const
		{
			return *m_commandBuffer;
		}
		/**
		*@~french
		*@return
		*	La barrière.
		*@~english
		*@return
		*	The fence.
		*/
		inline auto const & getFence()const
		{
			return m_fence;
		}
		/**
		*@~french
		*@return
		*	La périphérique logique.
		*@~english
		*@return
		*	The logical device.
		*/
		inline auto const & getDevice()const
		{
			return m_device;
		}

	private:
		void doCopyToStagingBuffer( uint8_t const * data
			, uint32_t size )const;

	private:
		vk::LogicalDevice const & m_device;
		vk::FrameBufferPtr m_frameBuffer;
		vk::PrimaryCommandBufferPtr m_commandBuffer;
		vk::SemaphorePtr m_imageAvailableSemaphore;
		vk::SemaphorePtr m_finishedRenderingSemaphore;
		vk::Fence m_fence;
		vk::StagingBufferPtr m_stagingBuffer;
	};
}

#endif
