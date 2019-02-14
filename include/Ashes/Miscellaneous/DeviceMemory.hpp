/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_DeviceMemory_HPP___
#define ___Ashes_DeviceMemory_HPP___
#pragma once

#include "Ashes/AshesPrerequisites.hpp"

#include "Ashes/Core/Device.hpp"

namespace ashes
{
	/**
	*\~english
	*\brief
	*	Computes an aligned size.
	*\param[in] size
	*	The unaligned size.
	*\param[in] align
	*	The alignment value.
	*\return
	*	The aligned size.
	*\~french
	*\brief
	*	Calcule une taille alignée.
	*\param[in] size
	*	La taille non alignée.
	*\param[in] align
	*	La valeur d'alignement.
	*\return
	*	La taille alignée.
	*/
	inline uint64_t getAlignedSize( uint64_t size, uint32_t align )
	{
		uint64_t result = 0u;

		while ( size > align )
		{
			size -= align;
			result += align;
		}

		return result + align;
	}
	/**
	*\~english
	*\brief
	*	A device memory object.
	*\~french
	*\brief
	*	Un objet mémoire.
	*/
	class DeviceMemory
	{
	protected:
		DeviceMemory( Device const & device
			, MemoryPropertyFlags flags )
			: m_device{ device }
			, m_flags{ flags }
		{
			registerObject( m_device, "DeviceMemory", this );
		}

	public:
		virtual ~DeviceMemory()
		{
			unregisterObject( m_device, this );
		}

		/**
		*\~english
		*\brief
		*	Maps a range of the memory in RAM.
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
		*	Mappe la mémoire en RAM.
		*\param[in] offset
		*	L'offset à partir duquel la mémoire est mappée.
		*\param[in] size
		*	La taille en octets de la mémoire à mapper.
		*\param[in] flags
		*	Indicateurs de configuration du mapping.
		*\return
		*	\p nullptr si le mapping a échoué.
		*/
		virtual uint8_t * lock( uint64_t offset
			, uint64_t size
			, ashes::MemoryMapFlags flags )const = 0;
		/**
		*\~english
		*\brief
		*	Invalidates the memory content.
		*\param[in] offset
		*	The mapped memory starting offset.
		*\param[in] size
		*	The range size.
		*\~french
		*\brief
		*	Invalide le contenu de la mémoire.
		*\param[in] offset
		*	L'offset de la mémoire mappée.
		*\param[in] size
		*	La taille en octets de la mémoire mappée.
		*/
		virtual void invalidate( uint64_t offset
			, uint64_t size )const = 0;
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
		virtual void flush( uint64_t offset
			, uint64_t size )const = 0;
		/**
		*\~english
		*\brief
		*	Unmaps the memory from RAM.
		*\~french
		*\brief
		*	Unmappe la mémoire de la RAM.
		*/
		virtual void unlock()const = 0;

	protected:
		Device const & m_device;
		MemoryPropertyFlags m_flags;
	};
}

#endif
