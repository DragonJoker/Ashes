/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "Gl4Renderer/GlRendererPrerequisites.hpp"

#include "Gl4Renderer/Enum/GlMemoryMapFlag.hpp"

namespace ashes::gl4
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
	{
	public:
		class DeviceMemoryImpl
		{
		public:
			DeviceMemoryImpl( VkDevice device
				, VkMemoryAllocateInfo allocateInfo
				, GLuint boundResource
				, GLuint boundTarget
				, VkDeviceSize memoryOffset );
			virtual ~DeviceMemoryImpl() = default;
			virtual VkResult lock( VkDeviceSize offset
				, VkDeviceSize size
				, VkMemoryMapFlags flags
				, void ** data )const = 0;
			virtual VkResult flush( VkDeviceSize offset
				, VkDeviceSize size )const = 0;
			virtual VkResult invalidate( VkDeviceSize offset
				, VkDeviceSize size )const = 0;
			virtual void unlock()const = 0;

		protected:
			VkDevice m_device;
			VkMemoryAllocateInfo m_allocateInfo;
			VkMemoryPropertyFlags m_flags;
			GlMemoryMapFlags m_mapFlags;
			GLuint m_boundResource;
			GLenum m_boundTarget;
			VkDeviceSize m_memoryOffset;
			declareDebugVariable( bool, m_isLocked, false );
		};

	public:
		DeviceMemory( VkDevice device
			, VkMemoryAllocateInfo allocateInfo );
		~DeviceMemory();
		VkResult bindToBuffer( VkBuffer buffer
			, VkDeviceSize memoryOffset );
		VkResult bindToImage( VkImage texture
			, VkDeviceSize memoryOffset );
		/**
		*\copydoc	DeviceMemory::lock
		*/
		VkResult lock( VkDeviceSize offset
			, VkDeviceSize size
			, VkMemoryMapFlags flags
			, void ** data )const;
		/**
		*\copydoc	DeviceMemory::flush
		*/
		VkResult flush( VkDeviceSize offset
			, VkDeviceSize size )const;
		/**
		*\copydoc	DeviceMemory::invalidate
		*/
		VkResult invalidate( VkDeviceSize offset
			, VkDeviceSize size )const;
		/**
		*\copydoc	DeviceMemory::unlock
		*/
		void unlock()const;

	private:
		VkDevice m_device;
		VkMemoryAllocateInfo m_allocateInfo;
		std::unique_ptr< DeviceMemoryImpl > m_impl;
	};
}
