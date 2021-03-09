/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#include "Miscellaneous/GlDeviceMemoryBinding.hpp"

namespace ashes::gl
{
	DeviceMemoryBinding::DeviceMemoryBinding( VkDeviceMemory parent
		, VkDevice device
		, GLuint boundTarget
		, VkDeviceSize memoryOffset
		, VkMemoryRequirements requirements
		, void * bound
		, GLuint boundName )
		: m_parent{ parent }
		, m_device{ device }
		, m_boundTarget{ boundTarget }
		, m_memoryOffset{ memoryOffset }
		, m_requirements{ std::move( requirements ) }
		, m_bound{ bound }
		, m_boundName{ boundName }
	{
	}

	void DeviceMemoryBinding::map( VkDeviceSize offset
		, VkDeviceSize size )
	{
		if ( size )
		{
			auto rngMin = std::max( offset, getOffset() );
			auto rngMax = std::min( offset + size, getOffset() + getSize() );
			m_mapped = rngMax > rngMin;

			if ( m_mapped )
			{
				m_mappedMin = rngMin;
				m_mappedMax = rngMax;
				m_dirty = true;
			}
		}
	}

	bool DeviceMemoryBinding::unmap()
	{
		m_mappedMin = 0;
		m_mappedMax = 0;
		m_mapped = false;
		auto dirty = m_dirty;
		m_dirty = false;
		return dirty;
	}

	void DeviceMemoryBinding::flush( VkDeviceSize offset
		, VkDeviceSize size )
	{
		if ( m_mapped && size )
		{
			auto rngMin = std::max( offset, getOffset() );
			auto rngMax = std::min( offset + size, getOffset() + getSize() );
			auto flushed = rngMax > rngMin;

			if ( flushed )
			{
				m_dirty = false;
			}
		}
	}

	void DeviceMemoryBinding::invalidate( VkDeviceSize offset
		, VkDeviceSize size )
	{
		if ( m_mapped && size )
		{
			auto rngMin = std::max( offset, getOffset() );
			auto rngMax = std::min( offset + size, getOffset() + getSize() );
			auto invalidated = rngMax > rngMin;

			if ( invalidated )
			{
				m_dirty = true;
			}
		}
	}

	void DeviceMemoryBinding::upload( ContextLock const & context
		, ByteArray const & data
		, VkDeviceSize offset
		, VkDeviceSize size )const
	{
	}
}
