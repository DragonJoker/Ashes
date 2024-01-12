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
		, GLuint boundName )noexcept
		: m_parent{ parent }
		, m_device{ device }
		, m_boundTarget{ boundTarget }
		, m_range{ memoryOffset, requirements.size }
		, m_requirements{ std::move( requirements ) }
		, m_bound{ bound }
		, m_boundName{ boundName }
	{
	}

	void DeviceMemoryBinding::map( BindingRange const & range )noexcept
	{
		if ( range )
		{
			auto inter = m_range.intersect( range );
			m_mapped = inter;

			if ( m_mapped )
			{
				m_mappedRange = inter;
				m_dirty = true;
			}
		}
	}

	bool DeviceMemoryBinding::unmap()noexcept
	{
		m_mappedRange = {};
		m_mapped = false;
		auto dirty = m_dirty;
		m_dirty = false;
		return dirty;
	}

	void DeviceMemoryBinding::flush( BindingRange const & range )noexcept
	{
		if ( m_mapped && range )
		{
			auto inter = m_range.intersect( range );
			auto flushed = inter;

			if ( flushed )
			{
				m_dirty = false;
			}
		}
	}

	void DeviceMemoryBinding::invalidate( BindingRange const & range )noexcept
	{
		if ( m_mapped && range )
		{
			auto inter = m_range.intersect( range );
			auto invalidated = inter;

			if ( invalidated )
			{
				m_dirty = true;
			}
		}
	}

	void DeviceMemoryBinding::upload( ContextLock const & context
		, ByteArray const & data
		, BindingRange const & range )const noexcept
	{
	}
}
