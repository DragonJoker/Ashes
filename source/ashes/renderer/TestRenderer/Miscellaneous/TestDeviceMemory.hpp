/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/TestRenderer/TestRendererPrerequisites.hpp"

#include <cassert>

namespace ashes::test
{
	class ObjectMemory
	{
		friend class DeviceMemory;

	public:
		ObjectMemory( ObjectMemory const & ) = delete;
		ObjectMemory & operator=( ObjectMemory const & ) = delete;
		ObjectMemory( VkDevice device
			, VkDeviceMemory deviceMemory
			, VkDeviceSize offset
			, VkMemoryAllocateInfo allocateInfo );
		ObjectMemory( ObjectMemory && rhs )noexcept;
		ObjectMemory & operator=( ObjectMemory && rhs )noexcept;
		~ObjectMemory()noexcept = default;

		VkResult lock( void ** mapped )const;
		void unlock()const noexcept;

	private:
		void upload( uint8_t const * buffer
			, VkDeviceSize offset
			, VkDeviceSize size )const;
		void download( uint8_t * buffer
			, VkDeviceSize offset
			, VkDeviceSize size )const;

	public:
		VkDevice device{};
		VkDeviceMemory deviceMemory{};
		VkDeviceSize offset{};
		VkDeviceSize size{};
		VkMemoryAllocateInfo allocateInfo{};
	};
	using ObjectMemoryPtr = std::unique_ptr< ObjectMemory >;
	/**
	*\brief
	*	Class wrapping a storage allocated to a data buffer.
	*/
	class DeviceMemory
		: public NonCopyable
	{
		friend class ObjectMemory;

	public:
		DeviceMemory( VkDevice device
			, VkMemoryAllocateInfo allocateInfo );
		~DeviceMemory()noexcept;
		VkResult bindToBuffer( VkBuffer buffer
			, VkDeviceSize memoryOffset
			, ObjectMemory *& objectMemory )noexcept;
		VkResult bindToImage1D( VkImage image
			, VkDeviceSize memoryOffset
			, ObjectMemory *& objectMemory )noexcept;
		VkResult bindToImage2D( VkImage image
			, VkDeviceSize memoryOffset
			, ObjectMemory *& objectMemory )noexcept;
		VkResult bindToImage3D( VkImage image
			, VkDeviceSize memoryOffset
			, ObjectMemory *& objectMemory )noexcept;

		void updateUpload( ObjectMemory const & memory
			, VkDeviceSize offset
			, VkDeviceSize size )const;
		void updateDownload( ObjectMemory const & memory
			, VkDeviceSize offset
			, VkDeviceSize size )const;
		void updateUpload( VkDeviceSize offset
			, VkDeviceSize size )const;
		void updateDownload( VkDeviceSize offset
			, VkDeviceSize size )const;
		void updateData( VkDeviceMemory src
			, VkDeviceSize srcOffset
			, VkDeviceSize dstOffset
			, VkDeviceSize size );

		VkResult lock( VkDeviceSize offset
			, VkDeviceSize size
			, VkMemoryMapFlags flags // Effectively expects the D3D11 Subresource index
			, void ** data )const;
		VkResult flush( VkDeviceSize offset
			, VkDeviceSize size )const;
		VkResult invalidate( VkDeviceSize offset
			, VkDeviceSize size )const;
		void unlock()const noexcept;

		bool isMapped()const noexcept
		{
			return m_mapped;
		}

		void upload()const noexcept
		{
			upload( m_mappedOffset, m_mappedSize );
		}

		void download()const noexcept
		{
			download( m_mappedOffset, m_mappedSize );
		}

		VkMemoryPropertyFlags getMemoryPropertyFlags()const noexcept
		{
			return m_propertyFlags;
		}

	public:
		mutable DeviceMemoryDestroySignal onDestroy;

	private:
		void upload( VkDeviceSize offset
			, VkDeviceSize size )const;
		void download( VkDeviceSize offset
			, VkDeviceSize size )const;

	private:
		VkDevice m_device{};
		VkMemoryAllocateInfo m_allocateInfo{};
		VkMemoryPropertyFlags m_propertyFlags{};
		std::vector< ObjectMemoryPtr > m_objects;
		mutable bool m_dirty{ true };
		mutable bool m_mapped{ false };
		mutable VkDeviceSize m_mappedOffset{ 0u };
		mutable VkDeviceSize m_mappedSize{ 0u };
		mutable ByteArray m_data;
	};
}
