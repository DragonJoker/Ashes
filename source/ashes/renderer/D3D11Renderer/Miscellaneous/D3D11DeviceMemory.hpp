/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/D3D11Renderer/D3D11RendererPrerequisites.hpp"

#include <cassert>

namespace ashes::D3D11_NAMESPACE
{
	class DeviceContextLock;

	class ObjectMemory
	{
		friend class DeviceMemory;

	public:
		ObjectMemory( ObjectMemory const & ) = delete;
		ObjectMemory & operator=( ObjectMemory const & ) = delete;
		ObjectMemory( VkDevice device
			, VkDeviceMemory deviceMemory
			, ID3D11Resource * resource
			, VkDeviceSize offset
			, VkMemoryAllocateInfo allocateInfo
			, D3D11_MAP mapFlags );
		ObjectMemory( ObjectMemory && rhs )noexcept;
		ObjectMemory & operator=( ObjectMemory && rhs )noexcept;
		~ObjectMemory()noexcept;

		VkResult lock( ID3D11DeviceContext * context
			, UINT subresource
			, D3D11_MAPPED_SUBRESOURCE & data )const;
		void unlock( ID3D11DeviceContext * context
			, UINT subresource )const;

	private:
		void upload( DeviceContextLock const & context
			, uint8_t const * data
			, UINT subresource
			, VkDeviceSize offset
			, VkDeviceSize size )const;
		void download( DeviceContextLock const & context
			, uint8_t * data
			, UINT subresource
			, VkDeviceSize offset
			, VkDeviceSize size )const;

	public:
		VkDevice device{};
		VkDeviceMemory deviceMemory{};
		ID3D11Resource * resource{};
		VkDeviceSize offset{};
		VkDeviceSize size{};
		VkMemoryAllocateInfo allocateInfo{};
		D3D11_MAP mapFlags{};
		std::vector< D3D11_SUBRESOURCE_DATA > subresources{};
	};
	using ObjectMemoryPtr = std::unique_ptr< ObjectMemory >;
	/**
	*\brief
	*	Class wrapping a storage allocated to a data buffer.
	*/
	class DeviceMemory
	{
	public:
		DeviceMemory( VkDevice device
			, VkMemoryAllocateInfo allocateInfo );
		~DeviceMemory()noexcept;
		VkResult bindToBuffer( VkBuffer buffer
			, VkDeviceSize memoryOffset
			, ObjectMemory *& objectMemory );
		VkResult bindToImage1D( VkImage image
			, VkDeviceSize memoryOffset
			, ObjectMemory *& objectMemory );
		VkResult bindToImage2D( VkImage image
			, VkDeviceSize memoryOffset
			, ObjectMemory *& objectMemory );
		VkResult bindToImage3D( VkImage image
			, VkDeviceSize memoryOffset
			, ObjectMemory *& objectMemory );

		void updateUpload( DeviceContextLock const & context
			, ObjectMemory const & memory
			, VkDeviceSize offset
			, VkDeviceSize size
			, UINT subresource )const;
		void updateDownload( DeviceContextLock const & context
			, ObjectMemory const & memory
			, VkDeviceSize offset
			, VkDeviceSize size
			, UINT subresource )const;
		void updateUpload( DeviceContextLock const & context
			, VkDeviceSize offset
			, VkDeviceSize size
			, UINT subresource )const;
		void updateDownload( DeviceContextLock const & context
			, VkDeviceSize offset
			, VkDeviceSize size
			, UINT subresource )const;
		void updateData( VkDeviceMemory src
			, VkDeviceSize srcOffset
			, VkDeviceSize dstOffset
			, VkDeviceSize size );

		VkResult lock( VkDeviceSize offset
			, VkDeviceSize size
			, VkMemoryMapFlags flags // Effectively expects the D3D11 Subresource index
			, void ** data )const;
		VkResult flush( DeviceContextLock const & context
			, VkDeviceSize offset
			, VkDeviceSize size )const;
		VkResult invalidate( DeviceContextLock const & context
			, VkDeviceSize offset
			, VkDeviceSize size )const;
		void unlock( DeviceContextLock const & context )const;

		bool isMapped()const
		{
			return m_mapped;
		}

		void upload( DeviceContextLock const & context )const
		{
			upload( context, m_mappedOffset, m_mappedSize );
		}

		void download( DeviceContextLock const & context )const
		{
			download( context, m_mappedOffset, m_mappedSize );
		}

		VkMemoryPropertyFlags getMemoryPropertyFlags()const
		{
			return m_propertyFlags;
		}

		VkDevice getDevice()const
		{
			return m_device;
		}

	public:
		mutable DeviceMemoryDestroySignal onDestroy;

	private:
		void upload( DeviceContextLock const & context
			, VkDeviceSize offset
			, VkDeviceSize size
			, UINT subresource )const;
		void download( DeviceContextLock const & context
			, VkDeviceSize offset
			, VkDeviceSize size
			, UINT subresource )const;

		void upload( DeviceContextLock const & context
			, VkDeviceSize offset
			, VkDeviceSize size )const
		{
			upload( context, offset, size, m_mappedSubresource );
		}

		void download( DeviceContextLock const & context
			, VkDeviceSize offset
			, VkDeviceSize size )const
		{
			download( context, offset, size, m_mappedSubresource );
		}

	private:
		VkDevice m_device{};
		VkMemoryAllocateInfo m_allocateInfo{};
		VkMemoryPropertyFlags m_propertyFlags{};
		VkMemoryRequirements m_requirements{};
		std::vector< ObjectMemoryPtr > m_objects{};
		mutable bool m_dirty{ true };
		mutable bool m_mapped{ false };
		mutable VkDeviceSize m_mappedOffset{ 0u };
		mutable VkDeviceSize m_mappedSize{ 0u };
		mutable UINT m_mappedSubresource{ 0u };
		mutable ByteArray m_data;
	};
}
