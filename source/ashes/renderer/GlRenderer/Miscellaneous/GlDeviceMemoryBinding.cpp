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
		m_mappedMin = std::max( offset, getOffset() );
		m_mappedMax = std::min( offset + size, getOffset() + getSize() );
		m_mapped = m_mappedMax > m_mappedMin;
	}

	void DeviceMemoryBinding::unmap()
	{
		m_mappedMin = 0;
		m_mappedMax = 0;
		m_mapped = false;
	}

	void DeviceMemoryBinding::upload( ContextLock const & context
		, ByteArray const & data
		, VkDeviceSize offset
		, VkDeviceSize size )const
	{
	}
}
