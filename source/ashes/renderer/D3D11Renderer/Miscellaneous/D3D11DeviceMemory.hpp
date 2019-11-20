/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/D3D11Renderer/D3D11RendererPrerequisites.hpp"

#include <cassert>

namespace ashes::d3d11
{
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
		void upload( uint8_t const * data
			, UINT subresource
			, VkDeviceSize offset
			, VkDeviceSize size )const;
		void download( uint8_t * data
			, UINT subresource
			, VkDeviceSize offset
			, VkDeviceSize size )const;

	public:
		VkDevice device;
		VkDeviceMemory deviceMemory;
		ID3D11Resource * resource;
		VkDeviceSize offset;
		VkDeviceSize size;
		VkMemoryAllocateInfo allocateInfo;
		D3D11_MAP mapFlags;
		std::vector< D3D11_SUBRESOURCE_DATA > subresources;
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
		~DeviceMemory();
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

		void updateUpload( ObjectMemory const & memory
			, VkDeviceSize offset
			, VkDeviceSize size
			, UINT subresource )const;
		void updateDownload( ObjectMemory const & memory
			, VkDeviceSize offset
			, VkDeviceSize size
			, UINT subresource )const;
		void updateUpload( VkDeviceSize offset
			, VkDeviceSize size
			, UINT subresource )const;
		void updateDownload( VkDeviceSize offset
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
		VkResult flush( VkDeviceSize offset
			, VkDeviceSize size )const;
		VkResult invalidate( VkDeviceSize offset
			, VkDeviceSize size )const;
		void unlock()const;

		inline bool isMapped()const
		{
			return m_mapped;
		}

		inline void upload()const
		{
			upload( m_mappedOffset, m_mappedSize );
		}

		inline void download()const
		{
			download( m_mappedOffset, m_mappedSize );
		}

		inline VkMemoryPropertyFlags getMemoryPropertyFlags()const
		{
			return m_propertyFlags;
		}

	public:
		mutable DeviceMemoryDestroySignal onDestroy;

	private:
		void upload( VkDeviceSize offset
			, VkDeviceSize size
			, UINT subresource )const;
		void download( VkDeviceSize offset
			, VkDeviceSize size
			, UINT subresource )const;

		void upload( VkDeviceSize offset
			, VkDeviceSize size )const
		{
			upload( offset, size, m_mappedSubresource );
		}

		void download( VkDeviceSize offset
			, VkDeviceSize size )const
		{
			download( offset, size, m_mappedSubresource );
		}

	private:
		VkDevice m_device;
		VkMemoryAllocateInfo m_allocateInfo;
		VkMemoryPropertyFlags m_propertyFlags;
		VkMemoryRequirements m_requirements;
		std::vector< ObjectMemoryPtr > m_objects;
		mutable bool m_dirty{ true };
		mutable bool m_mapped{ false };
		mutable VkDeviceSize m_mappedOffset{ 0u };
		mutable VkDeviceSize m_mappedSize{ 0u };
		mutable UINT m_mappedSubresource{ 0u };
		mutable ByteArray m_data;
	};
}
