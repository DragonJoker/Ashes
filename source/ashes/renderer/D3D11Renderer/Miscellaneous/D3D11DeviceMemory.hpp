/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/D3D11Renderer/D3D11RendererPrerequisites.hpp"

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

			void upload( ByteArray const & data
				, VkDeviceSize offset
				, VkDeviceSize size )const;
			void download( ByteArray & data
				, VkDeviceSize offset
				, VkDeviceSize size )const;

			virtual VkResult lock( VkDeviceSize offset
				, VkDeviceSize size
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

		void upload( VkDeviceSize offset
			, VkDeviceSize size )const;
		void download( VkDeviceSize offset
			, VkDeviceSize size )const;

		VkResult lock( VkDeviceSize offset
			, VkDeviceSize size
			, VkMemoryMapFlags flags
			, void ** data )const;
		VkResult flush( VkDeviceSize offset
			, VkDeviceSize size )const;
		VkResult invalidate( VkDeviceSize offset
			, VkDeviceSize size )const;
		void unlock()const;

		bool isMapped()const
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

	private:
		VkDevice m_device;
		VkMemoryAllocateInfo m_allocateInfo;
		VkMemoryPropertyFlags m_flags;
		VkMemoryRequirements m_requirements;
		std::unique_ptr< DeviceMemoryImpl > m_impl;
		mutable bool m_dirty = true;
		mutable bool m_mapped = false;
		mutable VkDeviceSize m_mappedOffset;
		mutable VkDeviceSize m_mappedSize;
		mutable ByteArray m_data;
	};
}
