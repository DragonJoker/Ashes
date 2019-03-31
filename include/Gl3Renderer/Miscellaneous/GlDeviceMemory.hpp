/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "Gl3Renderer/GlRendererPrerequisites.hpp"

#include <Ashes/Miscellaneous/DeviceMemory.hpp>
#include <Ashes/Miscellaneous/MemoryRequirements.hpp>

namespace gl_renderer
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
				, ashes::MemoryAllocateInfo allocateInfo
				, GLuint boundResource
				, GLuint boundTarget );
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
			ashes::MemoryAllocateInfo m_allocateInfo;
			ashes::MemoryPropertyFlags m_flags;
			GlMemoryMapFlags m_mapFlags;
			GLuint m_boundResource;
			GLenum m_boundTarget;
			declareDebugVariable( bool, m_isLocked, false );
		};

	public:
		DeviceMemory( Device const & device
			, ashes::MemoryAllocateInfo allocateInfo );
		~DeviceMemory();
		void bindToBuffer( GLuint resource, GLenum target );
		void bindToImage( Image const & texture
			, GLenum target
			, ashes::ImageCreateInfo const & createInfo );
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
		Device const & m_device;
		std::unique_ptr< DeviceMemoryImpl > m_impl;
	};
}
