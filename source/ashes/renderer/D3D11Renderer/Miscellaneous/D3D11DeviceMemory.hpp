/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "D3D11Renderer/D3D11RendererPrerequisites.hpp"

#include <Ashes/Miscellaneous/DeviceMemory.hpp>
#include <Ashes/Miscellaneous/MemoryRequirements.hpp>

namespace ashes::d3d11
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
		class DeviceMemoryImpl
		{
		public:
			DeviceMemoryImpl( VkDevice device
				, VkMemoryAllocateInfo allocateInfo );
			virtual ~DeviceMemoryImpl() = default;
			virtual uint8_t * lock( uint64_t offset
				, uint64_t size
				, VkMemoryMapFlags flags )const = 0;
			virtual void flush( uint64_t offset
				, uint64_t size )const = 0;
			virtual void invalidate( uint64_t offset
				, uint64_t size )const = 0;
			virtual void unlock()const = 0;

		protected:
			VkDevice m_device;
			VkMemoryAllocateInfo m_allocateInfo;
			VkMemoryPropertyFlags m_flags;
		};

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
		DeviceMemory( VkDevice device
			, VkMemoryAllocateInfo allocateInfo );
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
			, VkMemoryMapFlags flags )const;
		/**
		*\copydoc	ashes::DeviceMemory::flush
		*/
		void flush( uint64_t offset
			, uint64_t size )const;
		/**
		*\copydoc	ashes::DeviceMemory::invalidate
		*/
		void invalidate( uint64_t offset
			, uint64_t size )const;
		/**
		*\copydoc	ashes::DeviceMemory::unlock
		*/
		void unlock()const;

		ID3D11Buffer * bindToBuffer( VkBufferUsageFlags targets );
		ID3D11Texture1D * bindToTexture1D( VkImageCreateInfo const & createInfo );
		ID3D11Texture2D * bindToTexture2D( VkImageCreateInfo const & createInfo );
		ID3D11Texture3D * bindToTexture3D( VkImageCreateInfo const & createInfo );

	private:
		VkDevice m_device;
		VkMemoryRequirements m_requirements;
		std::unique_ptr< DeviceMemoryImpl > m_impl;
	};
}
