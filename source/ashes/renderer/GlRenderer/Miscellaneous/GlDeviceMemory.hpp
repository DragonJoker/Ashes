/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/GlRenderer/GlRendererPrerequisites.hpp"

#include "renderer/GlRenderer/Enum/GlMemoryMapFlag.hpp"

namespace ashes::gl
{
	class DeviceMemory
		: public IcdObject
	{
	public:
		class DeviceMemoryBinding
		{
		public:
			DeviceMemoryBinding( VkDeviceMemory parent
				, VkDevice device
				, GLenum boundTarget
				, VkDeviceSize memoryOffset
				, VkMemoryRequirements requirements
				, void * bound
				, GLuint boundName );
			virtual ~DeviceMemoryBinding();

			virtual void upload( ContextLock const & context
				, ByteArray const & data
				, VkDeviceSize offset
				, VkDeviceSize size )const;

			inline GLuint getInternal()const
			{
				return m_boundName;
			}

			inline void * getBound()const
			{
				return m_bound;
			}

			inline VkDeviceSize getSize()const
			{
				return m_requirements.size;
			}

			inline VkDeviceSize getAlignment()const
			{
				return m_requirements.alignment;
			}

		protected:
			VkDeviceMemory m_parent;
			VkDevice m_device;
			GLenum m_boundTarget;
			VkDeviceSize m_memoryOffset;
			VkMemoryRequirements m_requirements;
			void * m_bound;
			GLuint m_boundName;
		};
		using BindingPtr = std::unique_ptr< DeviceMemoryBinding >;

	public:
		DeviceMemory( VkDevice device
			, VkMemoryAllocateInfo allocateInfo );
		~DeviceMemory();
		VkResult bindBuffer( VkBuffer buffer
			, VkDeviceSize memoryOffset );
		VkResult bindImage( VkImage texture
			, VkDeviceSize memoryOffset );
		void unbindBuffer( VkBuffer buffer );
		void unbindImage( VkImage image );

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

		VkDeviceSize getSize()const
		{
			return m_allocateInfo.allocationSize;
		}

		inline VkDevice getDevice()const
		{
			return m_device;
		}

	public:
		mutable DeviceMemoryDestroySignal onDestroy;

	private:
		VkDevice m_device;
		VkMemoryAllocateInfo m_allocateInfo;
		VkMemoryPropertyFlags m_flags;
		GlMemoryMapFlags m_mapFlags;
		// Bindings, by offset
		std::vector< std::pair< VkDeviceSize, BindingPtr > > m_bindings;
		mutable bool m_dirty = true;
		mutable bool m_mapped = false;
		mutable VkDeviceSize m_mappedOffset;
		mutable VkDeviceSize m_mappedSize;
		mutable ByteArray m_data;
	};
}
