/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "TestRendererPrerequisites.hpp"

#include <Miscellaneous/DeviceMemory.hpp>
#include <Miscellaneous/MemoryRequirements.hpp>

namespace test_renderer
{
	/**
	*\~french
	*\brief
	*	Classe encapsulant le stockage alloué à un tampon de données.
	*\~english
	*\brief
	*	Class wrapping a storage allocated to a data buffer.
	*/
	class DeviceMemory
		: public ashes::DeviceMemory
	{
	public:
		/**
		*\~french
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le LogicalDevice parent.
		*\param[in] requirements
		*	Les exigences mémoire.
		*\param[in] flags
		*	Les indicateurs de propriétés voulues pour la mémoire allouée.
		*\~english
		*\brief
		*	Constructor.
		*\param[in] device
		*	The logical connection to the GPU.
		*\param[in] requirements
		*	The memory requirements.
		*\param[in] flags
		*	The wanted memory flags.
		*/
		DeviceMemory( Device const & device
			, ashes::MemoryRequirements const & requirements
			, ashes::MemoryPropertyFlags flags );
		/**
		*\~french
		*\brief
		*	Destructeur.
		*\~english
		*\brief
		*	Destructor.
		*/
		~DeviceMemory();
		/**
		*\copydoc	ashes::DeviceMemory::lock
		*/
		uint8_t * lock( uint64_t offset
			, uint64_t size
			, ashes::MemoryMapFlags flags )const override;
		/**
		*\copydoc	ashes::DeviceMemory::flush
		*/
		void flush( uint64_t offset
			, uint64_t size )const override;
		/**
		*\copydoc	ashes::DeviceMemory::invalidate
		*/
		void invalidate( uint64_t offset
			, uint64_t size )const override;
		/**
		*\copydoc	ashes::DeviceMemory::unlock
		*/
		void unlock()const override;

	private:
		mutable std::vector< uint8_t > m_memory;
	};
}
