/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/GlRenderer/GlRendererPrerequisites.hpp"

#include "renderer/GlRenderer/Enum/GlMemoryMapFlag.hpp"
#include "renderer/GlRenderer/Miscellaneous/GlDeviceMemoryBinding.hpp"

namespace ashes::gl
{
	class DeviceMemory
		: public IcdObject
	{
	public:
		DeviceMemory( VkAllocationCallbacks const * allocInfo
			, VkDevice device
			, VkMemoryAllocateInfo allocateInfo );
		~DeviceMemory();
		VkResult bindBuffer( VkBuffer buffer
			, VkDeviceSize memoryOffset );
		VkResult bindImage( VkImage texture
			, VkDeviceSize memoryOffset );
		void unbindBuffer( VkBuffer buffer );
		void unbindImage( VkImage image );

		void upload( ContextLock const & context
			, BindingRange const & range )const;
		void download( ContextLock const & context
			, BindingRange const & range )const;
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

		void upload( ContextLock const & context
			, VkDeviceSize offset
			, VkDeviceSize size )const
		{
			upload( context, { offset, size, getSize() } );
		}

		void download( ContextLock const & context
			, VkDeviceSize offset
			, VkDeviceSize size )const
		{
			download( context, { offset, size, getSize() } );
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
		std::vector< std::pair< VkDeviceSize, DeviceMemoryBindingPtr > > m_bindings;
		mutable bool m_dirty = true;
		mutable BindingRange m_mappedRange;
		mutable ByteArray m_data;
	};
}
