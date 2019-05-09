/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/D3D11Renderer/D3D11RendererPrerequisites.hpp"

#include <cassert>

namespace ashes::d3d11
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
				, VkMemoryAllocateInfo allocateInfo );
			virtual ~DeviceMemoryImpl() = default;

			virtual VkResult lock( ID3D11DeviceContext * context
				, UINT subresource
				, D3D11_MAPPED_SUBRESOURCE & data )const = 0;
			virtual void unlock( ID3D11DeviceContext * context
				, UINT subresource )const = 0;

			void upload( ByteArray const & data
				, UINT subresource
				, VkDeviceSize offset
				, VkDeviceSize size )const;
			void download( ByteArray & data
				, UINT subresource
				, VkDeviceSize offset
				, VkDeviceSize size )const;

		protected:
			VkDevice m_device;
			VkMemoryAllocateInfo m_allocateInfo;
			VkMemoryPropertyFlags m_propertyFlags;
		};

	public:
		DeviceMemory( VkDevice device
			, VkMemoryAllocateInfo allocateInfo );
		~DeviceMemory();
		VkResult bindToBuffer( VkBuffer buffer
			, VkDeviceSize memoryOffset
			, ID3D11Buffer *& retBuffer );
		VkResult bindToImage( VkImage image
			, VkDeviceSize memoryOffset
			, ID3D11Texture1D *& texture );
		VkResult bindToImage( VkImage image
			, VkDeviceSize memoryOffset
			, ID3D11Texture2D *& texture );
		VkResult bindToImage( VkImage image
			, VkDeviceSize memoryOffset
			, ID3D11Texture3D *& texture );

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

		inline bool hasBuffer()const
		{
			return m_buffer != nullptr;
		}

		inline ID3D11Buffer * getBuffer()const
		{
			return m_buffer;
		}

		inline void upload()const
		{
			upload( m_mappedOffset, m_mappedSize );
		}

		inline void download()const
		{
			download( m_mappedOffset, m_mappedSize );
		}

		inline DeviceMemoryImpl const & getImpl()const
		{
			assert( m_impl );
			return *m_impl;
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
		std::unique_ptr< DeviceMemoryImpl > m_impl;
		ID3D11Buffer * m_buffer{ nullptr };
		mutable bool m_dirty{ true };
		mutable bool m_mapped{ false };
		mutable VkDeviceSize m_mappedOffset{ 0u };
		mutable VkDeviceSize m_mappedSize{ 0u };
		mutable UINT m_mappedSubresource{ 0u };
		mutable ByteArray m_data;
	};
}
