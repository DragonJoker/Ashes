/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/GlRenderer/GlRendererPrerequisites.hpp"

#include "renderer/GlRenderer/Enum/GlMemoryMapFlag.hpp"

namespace ashes::gl
{
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
		virtual ~DeviceMemoryBinding() = default;

		void map( VkDeviceSize offset
			, VkDeviceSize size );
		void flush( VkDeviceSize offset
			, VkDeviceSize size );
		void invalidate( VkDeviceSize offset
			, VkDeviceSize size );
		bool unmap();

		virtual void upload( ContextLock const & context
			, ByteArray const & data
			, VkDeviceSize offset
			, VkDeviceSize size )const;

		GLuint getInternal()const
		{
			return m_boundName;
		}

		void * getBound()const
		{
			return m_bound;
		}

		VkDeviceSize getSize()const
		{
			return m_requirements.size;
		}

		VkDeviceSize getAlignment()const
		{
			return m_requirements.alignment;
		}

		VkDeviceSize getOffset()const
		{
			return m_memoryOffset;
		}

		bool isMapped()const
		{
			return m_mapped;
		}

		VkDeviceMemory getParent()const
		{
			return m_parent;
		}

	protected:
		VkDeviceMemory m_parent;
		VkDevice m_device;
		GLenum m_boundTarget;
		VkDeviceSize m_memoryOffset;
		VkMemoryRequirements m_requirements;
		void * m_bound;
		GLuint m_boundName;
		bool m_mapped = false;
		bool m_dirty = false;
		VkDeviceSize m_mappedMin;
		VkDeviceSize m_mappedMax;
	};
}
