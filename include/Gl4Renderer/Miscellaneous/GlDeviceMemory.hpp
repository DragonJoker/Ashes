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
			DeviceMemoryImpl( VkDeviceMemory parent
				, VkDevice device
				, VkMemoryAllocateInfo allocateInfo
				, GLuint boundResource
				, GLenum boundTarget
				, VkDeviceSize memoryOffset
				, GLuint buffer );
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
			virtual void unlock()const = 0;

			inline GLuint getInternal()const
			{
				return m_boundResource;
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
		};

	public:
		DeviceMemory( VkDevice device
			, VkMemoryAllocateInfo allocateInfo );
		~DeviceMemory();
		VkResult bindToBuffer( VkBuffer buffer
			, VkDeviceSize memoryOffset );
		VkResult bindToImage( VkImage texture
			, VkDeviceSize memoryOffset );

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

	private:
		VkDevice m_device;
		VkMemoryAllocateInfo m_allocateInfo;
		VkMemoryPropertyFlags m_flags;
		GlMemoryMapFlags m_mapFlags;
		std::unique_ptr< DeviceMemoryImpl > m_impl;
		GLuint m_buffer{ GL_INVALID_INDEX };
		mutable bool m_mapped = false;
		mutable VkDeviceSize m_mappedOffset;
		mutable VkDeviceSize m_mappedSize;
		mutable ByteArray m_data;
		mutable ByteArray m_sha;
	};
}
