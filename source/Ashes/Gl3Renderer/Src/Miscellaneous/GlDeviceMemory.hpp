/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "GlRendererPrerequisites.hpp"

#include <Miscellaneous/DeviceMemory.hpp>
#include <Miscellaneous/MemoryRequirements.hpp>

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
				, ashes::MemoryRequirements const & requirements
				, ashes::MemoryPropertyFlags flags
				, GLuint boundResource
				, GLuint boundTarget );
			virtual ~DeviceMemoryImpl() = default;
			virtual uint8_t * lock( uint32_t offset
				, uint32_t size
				, ashes::MemoryMapFlags flags )const = 0;
			virtual void flush( uint32_t offset
				, uint32_t size )const = 0;
			virtual void invalidate( uint32_t offset
				, uint32_t size )const = 0;
			virtual void unlock()const = 0;

		protected:
			Device const & m_device;
			ashes::MemoryRequirements m_requirements;
			ashes::MemoryPropertyFlags m_flags;
			GlMemoryMapFlags m_mapFlags;
			GLuint m_boundResource;
			GLenum m_boundTarget;
			declareDebugVariable( bool, m_isLocked, false );
		};

	public:
		DeviceMemory( Device const & device
			, ashes::MemoryRequirements const & requirements
			, ashes::MemoryPropertyFlags flags );
		~DeviceMemory();
		void bindToBuffer( GLuint resource, GLenum target );
		void bindToImage( Texture const & texture
			, GLenum target
			, ashes::ImageCreateInfo const & createInfo );
		/**
		*\copydoc	ashes::DeviceMemory::lock
		*/
		uint8_t * lock( uint32_t offset
			, uint32_t size
			, ashes::MemoryMapFlags flags )const override;
		/**
		*\copydoc	ashes::DeviceMemory::flush
		*/
		void flush( uint32_t offset
			, uint32_t size )const override;
		/**
		*\copydoc	ashes::DeviceMemory::invalidate
		*/
		void invalidate( uint32_t offset
			, uint32_t size )const override;
		/**
		*\copydoc	ashes::DeviceMemory::unlock
		*/
		void unlock()const override;

	private:
		Device const & m_device;
		ashes::MemoryRequirements m_requirements;
		std::unique_ptr< DeviceMemoryImpl > m_impl;
	};
}
