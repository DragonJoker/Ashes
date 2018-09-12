/*
This file belongs to RendererLib.
See LICENSE file in root folder
*/
#pragma once

#include "D3D11RendererPrerequisites.hpp"

#include <Miscellaneous/DeviceMemory.hpp>
#include <Miscellaneous/MemoryRequirements.hpp>

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
		: public renderer::DeviceMemory
	{
	public:
		class DeviceMemoryImpl
		{
		public:
			DeviceMemoryImpl( Device const & device
				, renderer::MemoryRequirements const & requirements
				, renderer::MemoryPropertyFlags flags );
			virtual ~DeviceMemoryImpl() = default;
			virtual uint8_t * lock( uint32_t offset
				, uint32_t size
				, renderer::MemoryMapFlags flags )const = 0;
			virtual void flush( uint32_t offset
				, uint32_t size )const = 0;
			virtual void invalidate( uint32_t offset
				, uint32_t size )const = 0;
			virtual void unlock()const = 0;

		protected:
			Device const & m_device;
			renderer::MemoryRequirements m_requirements;
			renderer::MemoryPropertyFlags m_flags;
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
			, renderer::MemoryRequirements const & requirements
			, renderer::MemoryPropertyFlags flags );
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
		*\copydoc	renderer::DeviceMemory::lock
		*/
		uint8_t * lock( uint32_t offset
			, uint32_t size
			, renderer::MemoryMapFlags flags )const override;
		/**
		*\copydoc	renderer::DeviceMemory::flush
		*/
		void flush( uint32_t offset
			, uint32_t size )const override;
		/**
		*\copydoc	renderer::DeviceMemory::invalidate
		*/
		void invalidate( uint32_t offset
			, uint32_t size )const override;
		/**
		*\copydoc	renderer::DeviceMemory::unlock
		*/
		void unlock()const override;

		ID3D11Buffer * bindToBuffer( renderer::BufferTargets targets );
		ID3D11Texture1D * bindToTexture1D( renderer::ImageCreateInfo const & createInfo );
		ID3D11Texture2D * bindToTexture2D( renderer::ImageCreateInfo const & createInfo );
		ID3D11Texture3D * bindToTexture3D( renderer::ImageCreateInfo const & createInfo );

	private:
		Device const & m_device;
		renderer::MemoryRequirements m_requirements;
		std::unique_ptr< DeviceMemoryImpl > m_impl;
	};
}
