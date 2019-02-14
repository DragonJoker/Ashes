/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "D3D11Renderer/D3D11RendererPrerequisites.hpp"

#include <Ashes/Miscellaneous/DeviceMemory.hpp>
#include <Ashes/Miscellaneous/MemoryRequirements.hpp>

namespace d3d11_renderer
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
			DeviceMemoryImpl( Device const & device
				, ashes::MemoryRequirements const & requirements
				, ashes::MemoryPropertyFlags flags );
			virtual ~DeviceMemoryImpl() = default;
			virtual uint8_t * lock( uint64_t offset
				, uint64_t size
				, ashes::MemoryMapFlags flags )const = 0;
			virtual void flush( uint64_t offset
				, uint64_t size )const = 0;
			virtual void invalidate( uint64_t offset
				, uint64_t size )const = 0;
			virtual void unlock()const = 0;

		protected:
			Device const & m_device;
			ashes::MemoryRequirements m_requirements;
			ashes::MemoryPropertyFlags m_flags;
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

		ID3D11Buffer * bindToBuffer( ashes::BufferTargets targets );
		ID3D11Texture1D * bindToTexture1D( ashes::ImageCreateInfo const & createInfo );
		ID3D11Texture2D * bindToTexture2D( ashes::ImageCreateInfo const & createInfo );
		ID3D11Texture3D * bindToTexture3D( ashes::ImageCreateInfo const & createInfo );

	private:
		Device const & m_device;
		ashes::MemoryRequirements m_requirements;
		std::unique_ptr< DeviceMemoryImpl > m_impl;
	};
}
