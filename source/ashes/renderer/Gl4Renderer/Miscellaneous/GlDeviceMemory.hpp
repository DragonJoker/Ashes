/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/Gl4Renderer/GlRendererPrerequisites.hpp"

#include "renderer/Gl4Renderer/Enum/GlMemoryMapFlag.hpp"

namespace ashes::gl4
{
	class DeviceMemory
		: public IcdObject
	{
	public:
		class DeviceMemoryImpl
		{
		public:
			DeviceMemoryImpl( VkDeviceMemory parent
				, VkDevice device
				, VkMemoryAllocateInfo allocateInfo
				, GLenum boundTarget
				, VkDeviceSize memoryOffset
				, VkDeviceSize align );
			virtual ~DeviceMemoryImpl();

			void upload( ContextLock const & context
				, ByteArray const & data
				, VkDeviceSize offset
				, VkDeviceSize size )const;
			void download( ContextLock const & context
				, ByteArray & data
				, VkDeviceSize offset
				, VkDeviceSize size )const;

			virtual VkResult lock( ContextLock const & context
				, VkDeviceSize offset
				, VkDeviceSize size
				, void ** data )const = 0;
			virtual void unlock( ContextLock const & context )const = 0;

			inline GLuint getInternal()const
			{
				return m_boundResource;
			}

			inline GLuint getBuffer()const
			{
				return m_buffer;
			}

		protected:
			VkDeviceMemory m_parent;
			VkDevice m_device;
			VkMemoryAllocateInfo m_allocateInfo;
			VkMemoryPropertyFlags m_flags;
			GlMemoryMapFlags m_mapFlags;
			GLuint m_buffer;
			GLuint m_boundResource;
			GLenum m_boundTarget;
			VkDeviceSize m_memoryOffset;
			VkDeviceSize m_align;
		};

	public:
		DeviceMemory( VkDevice device
			, VkMemoryAllocateInfo allocateInfo );
		~DeviceMemory();
		VkResult bindToBuffer( VkBuffer buffer
			, VkDeviceSize memoryOffset );
		VkResult bindToImage( VkImage texture
			, VkDeviceSize memoryOffset );

		void upload( ContextLock const & context
			, VkDeviceSize offset
			, VkDeviceSize size )const;
		void download( ContextLock const & context
			, VkDeviceSize offset
			, VkDeviceSize size )const;

		VkResult lock( ContextLock const & context
			, VkDeviceSize offset
			, VkDeviceSize size
			, VkMemoryMapFlags flags
			, void ** data )const;
		VkResult flush( ContextLock const & context
			, VkDeviceSize offset
			, VkDeviceSize size )const;
		VkResult invalidate( ContextLock const & context
			, VkDeviceSize offset
			, VkDeviceSize size )const;
		void unlock( ContextLock const & context )const;

		bool isMapped()const
		{
			return m_mapped;
		}

		GLuint getInternal()const
		{
			assert( m_impl );
			return m_impl->getInternal();
		}

		GLuint getBuffer()const
		{
			assert( m_impl );
			return m_impl->getBuffer();
		}

	public:
		mutable DeviceMemoryDestroySignal onDestroy;

	private:
		VkDevice m_device;
		VkMemoryAllocateInfo m_allocateInfo;
		VkMemoryPropertyFlags m_flags;
		GlMemoryMapFlags m_mapFlags;
		std::unique_ptr< DeviceMemoryImpl > m_impl;
		mutable bool m_dirty = true;
		mutable bool m_mapped = false;
		mutable VkDeviceSize m_mappedOffset;
		mutable VkDeviceSize m_mappedSize;
		mutable ByteArray m_data;
	};
}
