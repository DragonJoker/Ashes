/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_VertexBuffer_HPP___
#define ___Ashes_VertexBuffer_HPP___
#pragma once

#include "Ashes/Buffer/Buffer.hpp"
#include "Ashes/Miscellaneous/MemoryRequirements.hpp"

#include <vector>

namespace ashes
{
	/**
	*\brief
	*	Tampon de sommets.
	*/
	class VertexBufferBase
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le périphérique logique.
		*\param[in] size
		*	La taille du tampon.
		*\param[in] target
		*	Les indicateurs d'utilisation du tampon.
		*/
		VertexBufferBase( Device const & device
			, uint32_t size
			, BufferTargets target );
		/**
		*\~english
		*\brief
		*	Destructor.
		*\~french
		*\brief
		*	Destructeur.
		*/
		virtual ~VertexBufferBase() = default;
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
		*\return
		*	The memory requirements for this buffer.
		*\~french
		*\return
		*	Les exigences mémoire pour ce tampon.
		*/
		MemoryRequirements getMemoryRequirements()const;
		/**
		*\return
		*	La taille du tampon.
		*/
		inline uint32_t getSize()const
		{
			return m_size;
		}
		/**
		*\return
		*	Le tampon GPU.
		*/
		inline BufferBase const & getBuffer()const
		{
			return *m_buffer;
		}
		/**
		*\return
		*	Le périphérique logique.
		*/
		inline Device const & getDevice()const
		{
			return m_device;
		}

	protected:
		Device const & m_device;
		uint32_t m_size;
		BufferBasePtr m_buffer;
	};
	/**
	*\brief
	*	Tampon de sommets, typé.
	*/
	template< typename T >
	class VertexBuffer
		: public VertexBufferBase
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le périphérique logique.
		*\param[in] count
		*	La nombre d'éléments dans le tampon.
		*\param[in] target
		*	Les indicateurs d'utilisation du tampon.
		*/
		inline VertexBuffer( Device const & device
			, uint32_t count
			, BufferTargets target );
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
		inline void bindMemory( DeviceMemoryPtr memory )
		{
			m_buffer->bindMemory( std::move( memory ) );
		}
		/**
		*\~english
		*\return
		*	The memory requirements for this buffer.
		*\~french
		*\return
		*	Les exigences mémoire pour ce tampon.
		*/
		inline MemoryRequirements getMemoryRequirements()const
		{
			return m_buffer->getMemoryRequirements();
		}
		/**
		*\return
		*	Le nombre d'éléments contenus dans ce tampon.
		*/
		inline uint32_t getCount()const
		{
			return m_buffer->getSize() / sizeof( T );
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
		inline T * lock( uint64_t offset
			, uint64_t count
			, MemoryMapFlags flags )const
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
		inline void flush( uint64_t offset
			, uint64_t count )const
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
		inline void invalidate( uint64_t offset
			, uint64_t count )const
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
		virtual void unlock()const
		{
			m_buffer->unlock();
		}

	private:
		uint64_t doComputeSize( uint64_t count, uint64_t & offset )const
		{
			offset *= sizeof( T );
			count *= sizeof( T );
			auto const aligned = getAlignedSize( count
				, uint32_t( m_device.getProperties().limits.nonCoherentAtomSize ) );
			auto result = count == m_buffer->getSize()
				? ( offset == 0ull
					? ~( 0ull )
					: aligned )
				: ( offset + count == m_buffer->getSize()
					? count
					: aligned );
			assert( result == ~( 0ull ) || offset + result <= m_buffer->getSize() );
			return result;
		}
	};
	/**
	*\brief
	*	Fonction d'aide à la création d'un Buffer.
	*\param[in] device
	*	Le périphérique logique.
	*\param[in] count
	*	La nombre d'éléments dans le tampon.
	*\param[in] target
	*	Les indicateurs d'utilisation du tampon.
	*\param[in] flags
	*	Les indicateurs de mémoire du tampon.
	*\return
	*	Le tampon créé.
	*/
	template< typename T >
	inline VertexBufferPtr< T > makeVertexBuffer( Device const & device
		, uint32_t count
		, BufferTargets target )
	{
		return std::make_unique< VertexBuffer< T > >( device
			, count
			, target );
	}
}

#include "Ashes/Buffer/VertexBuffer.inl"

#endif
