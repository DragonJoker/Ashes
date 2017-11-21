/**
*\file
*	RenderingResources.h
*\author
*	Sylvain Doremus
*/
#ifndef ___Renderer_RenderingResources_HPP___
#define ___Renderer_RenderingResources_HPP___
#pragma once

#include <VkLib/FrameBuffer.hpp>
#include <VkLib/Fence.hpp>
#include <VkLib/PrimaryCommandBuffer.hpp>
#include <VkLib/Semaphore.hpp>

namespace renderer
{
	/**
	*@~french
	*@brief
	*	Classe regroupant les ressources de rendu nécessaires au dessin d'une image.
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
		*/
		bool waitRecord( uint32_t timeout );
		/**
		*@~french
		*@brief
		*	Copie les données d'une image dans une texture, en passant par le tampon de transfert.
		*@param[in] data
		*	Les données à copier.
		*@param[out] texture
		*	La texture de destination.
		*/
		void copyTextureData( vk::ByteArray const & data
			, vk::Image const & texture )const;
		/**
		*@~french
		*@brief
		*	Copie les données de sommets dans un tampon, en passant par le tampon de transfert.
		*@param[in] data
		*	Les données à copier.
		*@param[out] buffer
		*	Le tampon de destination.
		*/
		template< typename T >
		inline void copyBufferData( std::vector< T > const & data
			, vk::Buffer const & buffer )const
		{
			copyBufferData( reinterpret_cast< uint8_t const * const >( data.data() )
				, uint32_t( data.size() * sizeof( T ) )
				, 0u
				, buffer );
		}
		/**
		*@~french
		*@brief
		*	Copie les données de sommets dans un tampon, en passant par le tampon de transfert.
		*@param[in] data
		*	Les données à copier.
		*@param[out] buffer
		*	Le tampon de destination.
		*/
		inline void copyBufferData( vk::ByteArray const & data
			, vk::Buffer const & buffer )const
		{
			copyBufferData( data.data()
				, uint32_t( data.size() )
				, 0u
				, buffer );
		}
		/**
		*@~french
		*@brief
		*	Copie les données de sommets dans un tampon, en passant par le tampon de transfert.
		*@param[in] data
		*	Les données à copier.
		*@param[in] size
		*	La taille des données à copier.
		*@param[out] buffer
		*	Le tampon de destination.
		*/
		inline void copyBufferData( uint8_t const * const data
			, uint32_t size
			, vk::Buffer const & buffer )const
		{
			copyBufferData( data
				, size
				, 0u
				, buffer );
		}
		/**
		*@~french
		*@brief
		*	Copie les données de sommets dans un tampon, en passant par le tampon de transfert.
		*@param[in] data
		*	Les données à copier.
		*@param[in] offset
		*	L'offset dans le tampon.
		*@param[out] buffer
		*	Le tampon de destination.
		*/
		template< typename T >
		inline void copyBufferData( std::vector< T > const & data
			, uint32_t offset
			, vk::Buffer const & buffer )const
		{
			copyBufferData( reinterpret_cast< uint8_t const * const >( data.data() )
				, uint32_t( data.size() * sizeof( T ) )
				, offset
				, buffer );
		}
		/**
		*@~french
		*@brief
		*	Copie les données de sommets dans un tampon, en passant par le tampon de transfert.
		*@param[in] data
		*	Les données à copier.
		*@param[in] offset
		*	L'offset dans le tampon.
		*@param[out] buffer
		*	Le tampon de destination.
		*/
		inline void copyBufferData( vk::ByteArray const & data
			, uint32_t offset
			, vk::Buffer const & buffer )const
		{
			copyBufferData( data.data()
				, uint32_t( data.size() )
				, offset
				, buffer );
		}
		/**
		*@~french
		*@brief
		*	Copie les données de sommets dans un tampon, en passant par le tampon de transfert.
		*@param[in] data
		*	Les données à copier.
		*@param[in] size
		*	La taille des données à copier.
		*@param[in] offset
		*	L'offset dans le tampon.
		*@param[out] buffer
		*	Le tampon de destination.
		*/
		void copyBufferData( uint8_t const * const data
			, uint32_t size
			, uint32_t offset
			, vk::Buffer const & buffer )const;
		/**
		*@~french
		*@brief
		*	Copie les données de sommets dans un tampon, en passant par le tampon de transfert.
		*@param[in] data
		*	Les données à copier.
		*@param[out] buffer
		*	Le tampon de destination.
		*/
		template< typename T >
		inline void copyVertexData( std::vector< T > const & data
			, vk::VertexBuffer const & buffer )const
		{
			copyVertexData( reinterpret_cast< uint8_t const * const >( data.data() )
				, uint32_t( data.size() * sizeof( T ) )
				, 0u
				, buffer );
		}
		/**
		*@~french
		*@brief
		*	Copie les données de sommets dans un tampon, en passant par le tampon de transfert.
		*@param[in] data
		*	Les données à copier.
		*@param[out] buffer
		*	Le tampon de destination.
		*/
		inline void copyVertexData( vk::ByteArray const & data
			, vk::VertexBuffer const & buffer )const
		{
			copyVertexData( data.data()
				, uint32_t( data.size() )
				, 0u
				, buffer );
		}
		/**
		*@~french
		*@brief
		*	Copie les données de sommets dans un tampon, en passant par le tampon de transfert.
		*@param[in] data
		*	Les données à copier.
		*@param[in] size
		*	La taille des données à copier.
		*@param[out] buffer
		*	Le tampon de destination.
		*/
		inline void copyVertexData( uint8_t const * const data
			, uint32_t size
			, vk::VertexBuffer const & buffer )const
		{
			copyVertexData( data
				, size
				, 0u
				, buffer );
		}
		/**
		*@~french
		*@brief
		*	Copie les données de sommets dans un tampon, en passant par le tampon de transfert.
		*@param[in] data
		*	Les données à copier.
		*@param[in] offset
		*	L'offset dans le tampon.
		*@param[out] buffer
		*	Le tampon de destination.
		*/
		template< typename T >
		inline void copyVertexData( std::vector< T > const & data
			, uint32_t offset
			, vk::VertexBuffer const & buffer )const
		{
			copyVertexData( reinterpret_cast< uint8_t const * const >( data.data() )
				, uint32_t( data.size() * sizeof( T ) )
				, offset
				, buffer );
		}
		/**
		*@~french
		*@brief
		*	Copie les données de sommets dans un tampon, en passant par le tampon de transfert.
		*@param[in] data
		*	Les données à copier.
		*@param[in] offset
		*	L'offset dans le tampon.
		*@param[out] buffer
		*	Le tampon de destination.
		*/
		inline void copyVertexData( vk::ByteArray const & data
			, uint32_t offset
			, vk::VertexBuffer const & buffer )const
		{
			copyVertexData( data.data()
				, uint32_t( data.size() )
				, offset
				, buffer );
		}
		/**
		*@~french
		*@brief
		*	Copie les données de sommets dans un tampon, en passant par le tampon de transfert.
		*@param[in] data
		*	Les données à copier.
		*@param[in] size
		*	La taille des données à copier.
		*@param[in] offset
		*	L'offset dans le tampon.
		*@param[out] buffer
		*	Le tampon de destination.
		*/
		void copyVertexData( uint8_t const * const data
			, uint32_t size
			, uint32_t offset
			, vk::VertexBuffer const & buffer )const;
		/**
		*@~french
		*@brief
		*	Copie les données d'un tampon d'uniformes dans un tampon, en passant par le tampon de transfert.
		*@param[in] data
		*	Les données à copier.
		*@param[out] buffer
		*	Le tampon de destination.
		*/
		inline void copyUniformData( vk::ByteArray const & data
			, vk::UniformBuffer const & buffer )const
		{
			copyUniformData( data.data()
				, uint32_t( data.size() )
				, 0u
				, buffer );
		}
		/**
		*@~french
		*@brief
		*	Copie les données d'un tampon d'uniformes dans un tampon, en passant par le tampon de transfert.
		*@param[in] data
		*	Les données à copier.
		*@param[in] size
		*	La taille des données à copier.
		*@param[out] buffer
		*	Le tampon de destination.
		*/
		inline void copyUniformData( uint8_t const * const data
			, uint32_t size
			, vk::UniformBuffer const & buffer )const
		{
			copyUniformData( data
				, size
				, 0u
				, buffer );
		}
		/**
		*@~french
		*@brief
		*	Copie les données d'un tampon d'uniformes dans un tampon, en passant par le tampon de transfert.
		*@param[in] data
		*	Les données à copier.
		*@param[in] offset
		*	L'offset dans le tampon.
		*@param[out] buffer
		*	Le tampon de destination.
		*/
		void copyUniformData( vk::ByteArray const & data
			, uint32_t offset
			, vk::UniformBuffer const & buffer )const
		{
			copyUniformData( data.data()
				, uint32_t( data.size() )
				, offset
				, buffer );
		}
		/**
		*@~french
		*@brief
		*	Copie les données d'un tampon d'uniformes dans un tampon, en passant par le tampon de transfert.
		*@param[in] data
		*	Les données à copier.
		*@param[in] size
		*	La taille des données à copier.
		*@param[in] offset
		*	L'offset dans le tampon.
		*@param[out] buffer
		*	Le tampon de destination.
		*/
		void copyUniformData( uint8_t const * const data
			, uint32_t size
			, uint32_t offset
			, vk::UniformBuffer const & buffer )const;
		/**
		*@~french
		*@brief
		*	Définit le tampon d'images.
		*@param[in] frameBuffer
		*	Le nouveau tampon d'images.
		*/
		inline void setFrameBuffer( vk::FrameBufferPtr && frameBuffer )
		{
			m_frameBuffer = std::move( frameBuffer );
		}
		/**
		*@~french
		*@return
		*	Le tampon d'images.
		*/
		inline auto const & getFrameBuffer()const
		{
			return *m_frameBuffer;
		}
		/**
		*@~french
		*@return
		*	Le sémaphore d'attente que l'image soit disponible.
		*/
		inline auto const & getImageAvailableSemaphore()const
		{
			return *m_imageAvailableSemaphore;
		}
		/**
		*@~french
		*@return
		*	Le sémaphore d'attente que le rendu soit terminé.
		*/
		inline auto const & getRenderingFinishedSemaphore()const
		{
			return *m_finishedRenderingSemaphore;
		}
		/**
		*@~french
		*@return
		*	Le tampon de commandes.
		*/
		inline auto const & getCommandBuffer()const
		{
			return *m_commandBuffer;
		}
		/**
		*@~french
		*@return
		*	La barrière.
		*/
		inline auto const & getFence()const
		{
			return m_fence;
		}
		/**
		*@~french
		*@return
		*	La périphérique logique.
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
