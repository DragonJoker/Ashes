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
		~DeviceMemory()noexcept;
		VkResult bindBuffer( VkBuffer buffer
			, VkDeviceSize memoryOffset )noexcept;
		VkResult bindImage( VkImage texture
			, VkDeviceSize memoryOffset )noexcept;
		void unbindBuffer( VkBuffer buffer )noexcept;
		void unbindImage( VkImage image )noexcept;

		void upload( ContextLock const & context
			, BindingRange const & range )const noexcept;
		void download( ContextLock const & context
			, BindingRange const & range )const noexcept;
		VkResult lock( VkDeviceSize offset
			, VkDeviceSize size
			, VkMemoryMapFlags flags
			, void ** data )const noexcept;
		VkResult flush( ContextLock const & context
			, VkDeviceSize offset
			, VkDeviceSize size )const noexcept;
		VkResult invalidate( ContextLock const & context
			, VkDeviceSize offset
			, VkDeviceSize size )const noexcept;
		void unlock( ContextLock const & context )const noexcept;

		void upload( ContextLock const & context
			, VkDeviceSize offset
			, VkDeviceSize size )const noexcept
		{
			upload( context, { offset, size, getSize() } );
		}

		void download( ContextLock const & context
			, VkDeviceSize offset
			, VkDeviceSize size )const noexcept
		{
			download( context, { offset, size, getSize() } );
		}

		VkDeviceSize getSize()const noexcept
		{
			return m_allocateInfo.allocationSize;
		}

		VkDevice getDevice()const noexcept
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
