/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_Buffer_HPP___
#define ___AshesPP_Buffer_HPP___
#pragma once

#include "ashespp/Core/Device.hpp"
#include "ashespp/Miscellaneous/DeviceMemory.hpp"

#include <ashes/common/Exception.hpp>

namespace ashes
{
	/**
	*\~english
	*\brief
	*	GPU buffer class.
	*\~french
	*\brief
	*	Classe de tampon GPU.
	*/
	class BufferBase
	{
	public:
		/**
		*\~english
		*\brief
		*	Constructor.
		*\param[in] device
		*	The logical device.
		*\param[in] size
		*	The buffer size.
		*\param[in] target
		*	The buffer usage flags.
		*\~french
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le périphérique logique.
		*\param[in] size
		*	La taille du tampon.
		*\param[in] target
		*	Les indicateurs d'utilisation du tampon.
		*/
		BufferBase( Device const & device
			, VkDeviceSize size
			, VkBufferUsageFlags target );
		/**
		*\~english
		*\brief
		*	Destructor.
		*\~french
		*\brief
		*	Destructeur.
		*/
		~BufferBase();
		/**
		*\~english
		*\brief
		*	Binds this buffer to given device memory object.
		*\param[in] memory
		*	The memory object.
		*\~french
		*\brief
		*	Lie ce tampon à l'objet mémoire donné.
		*\param[in] memory
		*	L'object mémoire de périphérique.
		*/
		void bindMemory( DeviceMemoryPtr memory );
		/**
		*\~english
		*\brief
		*	Maps a range of the buffer's memory in RAM.
		*\param[in] offset
		*	The range beginning offset.
		*\param[in] size
		*	The range size.
		*\param[in] flags
		*	The mapping flags.
		*\return
		*	\p nullptr if mapping failed.
		*\~french
		*\brief
		*	Mappe la mémoire du tampon en RAM.
		*\param[in] offset
		*	L'offset à partir duquel la mémoire du tampon est mappée.
		*\param[in] size
		*	La taille en octets de la mémoire à mapper.
		*\param[in] flags
		*	Indicateurs de configuration du mapping.
		*\return
		*	\p nullptr si le mapping a échoué.
		*/
		uint8_t * lock( VkDeviceSize offset
			, VkDeviceSize size
			, VkMemoryMapFlags flags )const;
		/**
		*\~english
		*\brief
		*	Invalidates the buffer content.
		*\param[in] offset
		*	The mapped memory starting offset.
		*\param[in] size
		*	The range size.
		*\~french
		*\brief
		*	Invalide le contenu du tampon.
		*\param[in] offset
		*	L'offset de la mémoire mappée.
		*\param[in] size
		*	La taille en octets de la mémoire mappée.
		*/
		void invalidate( VkDeviceSize offset
			, VkDeviceSize size )const;
		/**
		*\~english
		*\brief
		*	Updates the VRAM.
		*\param[in] offset
		*	The mapped memory starting offset.
		*\param[in] size
		*	The range size.
		*\~french
		*\brief
		*	Met à jour la VRAM.
		*\param[in] offset
		*	L'offset de la mémoire mappée.
		*\param[in] size
		*	La taille en octets de la mémoire mappée.
		*/
		void flush( VkDeviceSize offset
			, VkDeviceSize size )const;
		/**
		*\~english
		*\brief
		*	Unmaps the buffer's memory from RAM.
		*\~french
		*\brief
		*	Unmappe la mémoire du tampon de la RAM.
		*/
		void unlock()const;
		/**
		*\~english
		*\return
		*	The compatible pipeline stage flags for current access.
		*\~french
		*\return
		*	Les indicateurs de niveau de pipeline pour les accès courants.
		*/
		inline VkPipelineStageFlags getCompatibleStageFlags()const
		{
			return m_compatibleStageFlags;
		}
		/**
		*\~english
		*\brief
		*	Prepares a buffer memory barrier, to a transfer destination layout.
		*\return
		*	The memory barrier.
		*\~french
		*\brief
		*	Prépare une barrière mémoire de transition vers un layout de destination de transfert.
		*\return
		*	La barrière mémoire.
		*/
		VkBufferMemoryBarrier makeTransferDestination( uint32_t srcQueueFamily = ~( 0u )
			, uint32_t dstQueueFamily = ~( 0u ) )const;
		/**
		*\~english
		*\brief
		*	Prepares a buffer memory barrier, to a transfer source layout.
		*\return
		*	The memory barrier.
		*\~french
		*\brief
		*	Prépare une barrière mémoire de transition vers un layout de source de transfert.
		*\return
		*	La barrière mémoire.
		*/
		VkBufferMemoryBarrier makeTransferSource( uint32_t srcQueueFamily = ~( 0u )
			, uint32_t dstQueueFamily = ~( 0u ) )const;
		/**
		*\~english
		*\brief
		*	Prepares a buffer memory barrier, to a vertex shader input layout.
		*\return
		*	The memory barrier.
		*\~french
		*\brief
		*	Prépare une barrière mémoire de transition vers un layout de ressource d'entrée (lecture seule) d'un vertex shader.
		*\return
		*	La barrière mémoire.
		*/
		VkBufferMemoryBarrier makeVertexShaderInputResource( uint32_t srcQueueFamily = ~( 0u )
			, uint32_t dstQueueFamily = ~( 0u ) )const;
		/**
		*\~english
		*\brief
		*	Prepares a buffer memory barrier, to a uniform buffer input layout.
		*\return
		*	The memory barrier.
		*\~french
		*\brief
		*	Prépare une barrière mémoire de transition vers un layout de ressource d'entrée (lecture seule) d'un shader.
		*\return
		*	La barrière mémoire.
		*/
		VkBufferMemoryBarrier makeUniformBufferInput( uint32_t srcQueueFamily = ~( 0u )
			, uint32_t dstQueueFamily = ~( 0u ) )const;
		/**
		*\~english
		*\brief
		*	Prepares a buffer memory barrier, to a given memory layout.
		*\param[in] dstAccessFlags
		*	The wanted access flags, after the transition.
		*\param[in] srcQueueFamily
		*	The source queue family, if a responsibility transfer must happen.
		*\param[in] dstQueueFamily
		*	The destination queue family, if a responsibility transfer must happen.
		*\return
		*	The memory barrier.
		*\~french
		*\brief
		*	Prépare une barrière mémoire de transition vers un layout mémoire donné.
		*\param[in] dstAccessFlags
		*	Les indicateurs d'accès voulus après la transition.
		*\param[in] srcQueueFamily
		*	La famille de file source, si un transfert de responsabilité doit être effectué.
		*\param[in] dstQueueFamily
		*	La famille de file destination, si un transfert de responsabilité doit être effectué..
		*\return
		*	La barrière mémoire.
		*/
		VkBufferMemoryBarrier makeMemoryTransitionBarrier( VkAccessFlags dstAccessFlags
			, uint32_t srcQueueFamily = ~( 0u )
			, uint32_t dstQueueFamily = ~( 0u ) )const;
		/**
		*\~english
		*\return
		*	The memory requirements for this buffer.
		*\~french
		*\return
		*	Les exigences mémoire pour ce tampon.
		*/
		 VkMemoryRequirements getMemoryRequirements()const;
		/**
		*\~english
		*\return
		*	The buffer size.
		*\~french
		*\return
		*	La taille du tampon.
		*/
		inline VkDeviceSize getSize()const
		{
			return m_size;
		}
		/**
		*\~english
		*\return
		*	The buffer usage flags.
		*\~french
		*\return
		*	Les cibles du tampon.
		*/
		inline VkBufferUsageFlags getTargets()const
		{
			return m_target;
		}
		/**
		*\~french
		*\brief
		*	Conversion implicite vers VkBuffer.
		*\~english
		*\brief
		*	VkBuffer implicit cast operator.
		*/
		inline operator VkBuffer const & ()const
		{
			return m_internal;
		}

	private:
		Device const & m_device;
		VkDeviceSize m_size;
		VkBufferUsageFlags m_target;
		VkBuffer m_internal{ VK_NULL_HANDLE };
		DeviceMemoryPtr m_storage;
		mutable VkAccessFlags m_currentAccessFlags{ VK_ACCESS_MEMORY_WRITE_BIT };
		mutable VkPipelineStageFlags m_compatibleStageFlags{ VK_PIPELINE_STAGE_HOST_BIT };
	};
	/**
	*\~english
	*\brief
	*	Template class wrapping a GPU buffer, to store structured Data.
	*\~french
	*\brief
	*	Classe template encapsulant un tampon GPU, pour stocker des données structurées.
	*/
	template< typename T >
	class Buffer
	{
	public:
		/**
		*\~english
		*\brief
		*	Constructor.
		*\param[in] device
		*	The logical device.
		*\param[in] count
		*	The buffer elements count.
		*\param[in] target
		*	The buffer usage flags.
		*\~french
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le périphérique logique.
		*\param[in] count
		*	Le nombre d'éléments du tampon.
		*\param[in] target
		*	Les indicateurs d'utilisation du tampon.
		*/
		Buffer( Device const & device
			, VkDeviceSize count
			, VkBufferUsageFlags target );
		/**
		*\~english
		*\return
		*	The memory requirements for this buffer.
		*\~french
		*\return
		*	Les exigences mémoire pour ce tampon.
		*/
		inline VkMemoryRequirements getMemoryRequirements()const
		{
			return m_buffer->getMemoryRequirements();
		}
		/**
		*\~english
		*\brief
		*	Binds this buffer to given device memory object.
		*\param[in] memory
		*	The memory object.
		*\~french
		*\brief
		*	Lie ce tampon à l'objet mémoire donné.
		*\param[in] memory
		*	L'object mémoire de périphérique.
		*/
		void bindMemory( DeviceMemoryPtr memory )
		{
			m_buffer->bindMemory( std::move( memory ) );
		}
		/**
		*\~english
		*\return
		*	The elements count.
		*\~french
		*\return
		*	Le nombre d'éléments.
		*/
		inline VkDeviceSize getCount()const
		{
			return m_buffer->getSize() / sizeof( T );
		}
		/**
		*\~english
		*\return
		*	The GPU buffer.
		*\~french
		*\return
		*	Le tampon GPU.
		*/
		inline BufferBase const & getBuffer()const
		{
			return *m_buffer;
		}
		/**
		*\~english
		*\brief
		*	Maps a range of the buffer's memory in RAM.
		*\param[in] offset
		*	The range elements beginning offset.
		*\param[in] count
		*	The range elements count.
		*\param[in] flags
		*	The mapping flags.
		*\return
		*	\p nullptr if mapping failed.
		*\~french
		*\brief
		*	Mappe la mémoire du tampon en RAM.
		*\param[in] offset
		*	L'offset à partir duquel les éléments du tampon sont mappés.
		*\param[in] count
		*	Le nombre d'éléments à mapper.
		*\param[in] flags
		*	Indicateurs de configuration du mapping.
		*\return
		*	\p nullptr si le mapping a échoué.
		*/
		inline T * lock( VkDeviceSize offset
			, VkDeviceSize count
			, VkMemoryMapFlags flags )const
		{
			auto size = doComputeSize( count, offset );
			return reinterpret_cast< T * >( m_buffer->lock( offset, size, flags ) );
		}
		/**
		*\~english
		*\brief
		*	Updates the VRAM.
		*\param[in] offset
		*	The mapped elements starting offset.
		*\param[in] count
		*	The range elements count.
		*\~french
		*\brief
		*	Met à jour la VRAM.
		*\param[in] offset
		*	L'offset des éléments mappés.
		*\param[in] count
		*	Le nombre d'éléments mappés.
		*/
		inline void flush( VkDeviceSize offset
			, VkDeviceSize count )const
		{
			auto size = doComputeSize( count, offset );
			m_buffer->flush( offset, size );
		}
		/**
		*\~english
		*\brief
		*	Invalidates the buffer content.
		*\param[in] offset
		*	The mapped elements starting offset.
		*\param[in] count
		*	The range elements count.
		*\~french
		*\brief
		*	Invalide le contenu du tampon.
		*\param[in] offset
		*	L'offset des éléments mappés.
		*\param[in] count
		*	Le nombre d'éléments mappés.
		*/
		inline void invalidate( VkDeviceSize offset
			, VkDeviceSize count )const
		{
			auto size = doComputeSize( count, offset );
			m_buffer->invalidate( offset, size );
		}
		/**
		*\~english
		*\brief
		*	Unmaps the buffer's memory from RAM.
		*\~french
		*\brief
		*	Unmappe la mémoire du tampon de la RAM.
		*/
		void unlock()const
		{
			m_buffer->unlock();
		}
		/**
		*\~french
		*\brief
		*	Conversion implicite vers VkBuffer.
		*\~english
		*\brief
		*	VkBuffer implicit cast operator.
		*/
		inline operator VkBuffer const & ()const
		{
			return *m_buffer;
		}

	private:
		uint64_t doComputeSize( VkDeviceSize count, VkDeviceSize & offset )const
		{
			offset *= sizeof( T );
			count *= sizeof( T );
			auto const aligned = getAlignedSize( count
				, m_device.getProperties().limits.nonCoherentAtomSize );
			auto result = count == m_buffer->getSize()
				? ( offset == VkDeviceSize( 0 )
					? WholeSize
					: aligned )
				: ( offset + count == m_buffer->getSize()
					? count
					: aligned );
			assert( result == WholeSize || offset + result <= m_buffer->getSize() );
			return result;
		}

	private:
		Device const & m_device;
		BufferBasePtr m_buffer;
	};
	/**
	*\~english
	*\brief
	*	Helper function tor create a Buffer< T >.
	*\param[in] device
	*	The logical device.
	*\param[in] count
	*	The buffer elements count.
	*\param[in] target
	*	The buffer usage flags.
	*\param[in] flags
	*	The buffer memory flags.
	*\return
	*	The created buffer.
	*\~french
	*\brief
	*	Fonction d'aide à la création d'un Buffer< T >.
	*\param[in] device
	*	Les périphérique logique.
	*\param[in] count
	*	Le nombre d'éléments du tampon.
	*\param[in] target
	*	Les indicateurs d'utilisation du tampon.
	*\param[in] flags
	*	Les indicateurs de mémoire du tampon.
	*\return
	*	Le tampon créé.
	*/
	template< typename T >
	BufferPtr< T > makeBuffer( Device const & device
		, VkDeviceSize count
		, VkBufferUsageFlags target )
	{
		return std::make_unique< Buffer< T > >( device
			, count
			, target );
	}
}

#include "ashespp/Buffer/Buffer.inl"

#endif
