/**
*\file
*	RenderingResources.h
*\author
*	Sylvain Doremus
*/
#ifndef ___Renderer_StagingBuffer_HPP___
#define ___Renderer_StagingBuffer_HPP___
#pragma once

#include "Buffer.hpp"
#include "VertexBuffer.hpp"
#include "UniformBuffer.hpp"

#include <VkLib/StagingBuffer.hpp>
#include <VkLib/UniformBuffer.hpp>
#include <VkLib/VertexBuffer.hpp>

namespace renderer
{
	/**
	*@~french
	*@brief
	*	Classe regroupant les ressources de rendu nécessaires au dessin d'une image.
	*/
	class StagingBuffer
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
		StagingBuffer( Device const & device );
		/**
		*\brief
		*	Prépare une barrière mémoire de transition vers un layout de source de transfert.
		*\param[in] dstAccess
		*	Les indicateurs d'accès voulus après la transition.
		*\return
		*	La barrière mémoire.
		*/
		BufferMemoryBarrier makeTransferSource()const;
		/**
		*@~french
		*@brief
		*	Copie les données d'une image dans une texture, en passant par le tampon de transfert.
		*@param[in] data
		*	Les données à copier.
		*@param[out] texture
		*	La texture de destination.
		*/
		void copyTextureData( CommandBuffer const & commandBuffer
			, ByteArray const & data
			, Texture const & texture )const;
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
		inline void copyBufferData( CommandBuffer const & commandBuffer
			, std::vector< T > const & data
			, Buffer< T > const & buffer )const
		{
			copyBufferData( commandBuffer
				, reinterpret_cast< uint8_t const * const >( data.data() )
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
		template< typename T >
		inline void copyBufferData( CommandBuffer const & commandBuffer
			, ByteArray const & data
			, Buffer< T > const & buffer )const
		{
			copyBufferData( commandBuffer
				, data.data()
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
		template< typename T >
		inline void copyBufferData( CommandBuffer const & commandBuffer
			, uint8_t const * const data
			, uint32_t size
			, Buffer< T > const & buffer )const
		{
			copyBufferData( commandBuffer
				, data
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
		inline void copyBufferData( CommandBuffer const & commandBuffer
			, std::vector< T > const & data
			, uint32_t offset
			, Buffer< T > const & buffer )const
		{
			copyBufferData( commandBuffer
				, reinterpret_cast< uint8_t const * const >( data.data() )
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
		template< typename T >
		inline void copyBufferData( CommandBuffer const & commandBuffer
			, ByteArray const & data
			, uint32_t offset
			, Buffer< T > const & buffer )const
		{
			copyBufferData( commandBuffer
				, data.data()
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
		template< typename T >
		void copyBufferData( CommandBuffer const & commandBuffer
			, uint8_t const * const data
			, uint32_t size
			, uint32_t offset
			, Buffer< T > const & buffer )const
		{
			doCopyToStagingBuffer( commandBuffer
				, data
				, size );
			doCopyFromStagingBuffer( commandBuffer
				, size
				, offset
				, buffer.getBuffer() );
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
		template< typename T >
		inline void copyVertexData( CommandBuffer const & commandBuffer
			, std::vector< T > const & data
			, VertexBuffer< T > const & buffer )const
		{
			copyVertexData( commandBuffer
				, reinterpret_cast< uint8_t const * const >( data.data() )
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
		template< typename T >
		inline void copyVertexData( CommandBuffer const & commandBuffer
			, ByteArray const & data
			, VertexBuffer< T > const & buffer )const
		{
			copyVertexData( commandBuffer
				, data.data()
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
		template< typename T >
		inline void copyVertexData( CommandBuffer const & commandBuffer
			, uint8_t const * const data
			, uint32_t size
			, VertexBuffer< T > const & buffer )const
		{
			copyVertexData( commandBuffer
				, data
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
		inline void copyVertexData( CommandBuffer const & commandBuffer
			, std::vector< T > const & data
			, uint32_t offset
			, VertexBuffer< T > const & buffer )const
		{
			copyVertexData( commandBuffer
				, reinterpret_cast< uint8_t const * const >( data.data() )
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
		template< typename T >
		inline void copyVertexData( CommandBuffer const & commandBuffer
			, ByteArray const & data
			, uint32_t offset
			, VertexBuffer< T > const & buffer )const
		{
			copyVertexData( commandBuffer
				, data.data()
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
		template< typename T >
		void copyVertexData( CommandBuffer const & commandBuffer
			, uint8_t const * const data
			, uint32_t size
			, uint32_t offset
			, VertexBuffer< T > const & buffer )const
		{
			doCopyToStagingBuffer( commandBuffer
				, data
				, size );
			doCopyFromStagingBuffer( commandBuffer
				, size
				, offset
				, buffer.getVbo().getBuffer() );
		}
		/**
		*@~french
		*@brief
		*	Copie les données d'un tampon d'uniformes dans un tampon, en passant par le tampon de transfert.
		*@param[in] data
		*	Les données à copier.
		*@param[out] buffer
		*	Le tampon de destination.
		*/
		template< typename T >
		inline void copyUniformData( CommandBuffer const & commandBuffer
			, ByteArray const & data
			, UniformBuffer< T > const & buffer )const
		{
			copyUniformData( commandBuffer
				, data.data()
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
		template< typename T >
		inline void copyUniformData( CommandBuffer const & commandBuffer
			, uint8_t const * const data
			, uint32_t size
			, UniformBuffer< T > const & buffer )const
		{
			copyUniformData( commandBuffer
				, data
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
		template< typename T >
		void copyUniformData( CommandBuffer const & commandBuffer
			, ByteArray const & data
			, uint32_t offset
			, UniformBuffer< T > const & buffer )const
		{
			copyUniformData( commandBuffer
				, data.data()
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
		*@param[out] buffer
		*	Le tampon de destination.
		*/
		template< typename T >
		void copyUniformData( CommandBuffer const & commandBuffer
			, std::vector< T > const & data
			, UniformBuffer< T > const & buffer )const
		{
			copyUniformData( commandBuffer
				, reinterpret_cast< uint8_t const * const >( data.data() )
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
		template< typename T >
		void copyUniformData( CommandBuffer const & commandBuffer
			, T const * const data
			, uint32_t size
			, UniformBuffer< T > const & buffer )const
		{
			copyUniformData( commandBuffer
				, reinterpret_cast< uint8_t const * const >( data )
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
		template< typename T >
		void copyUniformData( CommandBuffer const & commandBuffer
			, std::vector< T > const & data
			, uint32_t offset
			, UniformBuffer< T > const & buffer )const
		{
			copyUniformData( commandBuffer
				, reinterpret_cast< uint8_t const * const >( data.data() )
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
		template< typename T >
		void copyUniformData( CommandBuffer const & commandBuffer
			, T const * const data
			, uint32_t size
			, uint32_t offset
			, UniformBuffer< T > const & buffer )const
		{
			copyUniformData( commandBuffer
				, reinterpret_cast< uint8_t const * const >( data )
				, size
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
		template< typename T >
		void copyUniformData( CommandBuffer const & commandBuffer
			, uint8_t const * const data
			, uint32_t size
			, uint32_t offset
			, UniformBuffer< T > const & buffer )const
		{
			doCopyToStagingBuffer( commandBuffer
				, data
				, size );
			doCopyFromStagingBuffer( commandBuffer
				, size
				, offset
				, buffer.getUbo().getBuffer() );
		}
		/**
		*\return
		*	Le tampon de transfert vulkan.
		*/
		vk::StagingBuffer const & getStagingBuffer()const
		{
			return *m_stagingBuffer;
		}

	private:
		void doCopyToStagingBuffer( CommandBuffer const & commandBuffer
			, uint8_t const * data
			, uint32_t size )const;
		void doCopyFromStagingBuffer( CommandBuffer const & commandBuffer
			, uint32_t size
			, uint32_t offset
			, vk::Buffer const & buffer )const;

	private:
		Device const & m_device;
		vk::StagingBufferPtr m_stagingBuffer;
		AccessFlags m_currentAccessMask{ AccessFlag::eMemoryWrite };
	};
}

#endif
