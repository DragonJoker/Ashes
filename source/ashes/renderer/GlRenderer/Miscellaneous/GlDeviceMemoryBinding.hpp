/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/GlRenderer/GlRendererPrerequisites.hpp"

#include "renderer/GlRenderer/Enum/GlMemoryMapFlag.hpp"

namespace ashes::gl
{
	struct BindingRange
	{
		BindingRange( VkDeviceSize offset = 0u
			, VkDeviceSize size = 0u )
			: m_offset{ offset }
			, m_size{ size }
		{
		}

		BindingRange( VkDeviceSize offset
			, VkDeviceSize size
			, VkDeviceSize maxSize )
			: m_offset{ offset }
			, m_size{ size }
		{
			if ( m_size == WholeSize )
			{
				m_size = maxSize;
				m_offset = 0u;
			}
		}

		BindingRange intersect( BindingRange const & range )const
		{
			auto rngMin = std::max( range.getMin(), getMin() );
			auto rngMax = std::min( range.getMax(), getMax() );
			return ( rngMax > rngMin
				? BindingRange{ rngMin, rngMax - rngMin }
				: BindingRange{ 0u, 0u } );
		}

		operator bool()const
		{
			return getMax() > getMin();
		}

		VkDeviceSize getOffset()const
		{
			return m_offset;
		}

		VkDeviceSize getSize()const
		{
			return m_size;
		}

		VkDeviceSize getMin()const
		{
			return m_offset;
		}

		VkDeviceSize getMax()const
		{
			return m_offset + m_size;
		}

	private:
		VkDeviceSize m_offset;
		VkDeviceSize m_size;
	};

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

		void map( BindingRange const & range );
		void flush( BindingRange const & range );
		void invalidate( BindingRange const & range );
		bool unmap();

		virtual void upload( ContextLock const & context
			, ByteArray const & data
			, BindingRange const & range )const;

		GLuint getInternal()const
		{
			return m_boundName;
		}

		void * getBound()const
		{
			return m_bound;
		}

		VkDeviceSize getAlignment()const
		{
			return m_requirements.alignment;
		}

		VkDeviceSize getSize()const
		{
			return m_range.getSize();
		}

		VkDeviceSize getOffset()const
		{
			return m_range.getOffset();
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
		BindingRange m_range;
		VkMemoryRequirements m_requirements;
		void * m_bound;
		GLuint m_boundName;
		bool m_mapped = false;
		bool m_dirty = false;
		BindingRange m_mappedRange;
	};
}
