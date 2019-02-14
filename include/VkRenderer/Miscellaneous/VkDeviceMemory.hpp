/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "VkRenderer/VkRendererPrerequisites.hpp"

#include <Ashes/Miscellaneous/DeviceMemory.hpp>

namespace vk_renderer
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
		/**
		*\~french
		*\brief
		*	Opérateur de conversion implicite vers VkDeviceMemory.
		*\~english
		*\brief
		*	VkDeviceMemory implicit cast operator.
		*/
		inline operator VkDeviceMemory const &()const
		{
			return m_memory;
		}

	private:
		Device const & m_device;
		VkDeviceMemory m_memory{};
	};
}
