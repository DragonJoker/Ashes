/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#include "Miscellaneous/VkDeviceMemory.hpp"

#include "Core/VkDevice.hpp"
#include "Core/VkPhysicalDevice.hpp"

namespace vk_renderer
{
	DeviceMemory::DeviceMemory( Device const & device
		, ashes::MemoryRequirements const & requirements
		, ashes::MemoryPropertyFlags flags )
		: ashes::DeviceMemory{ device, flags }
		, m_device{ device }
	{
		uint32_t deducedTypeIndex{ 0xFFFFFFFF };

		if ( !m_device.getPhysicalDevice().deduceMemoryType( requirements.memoryTypeBits
			, flags
			, deducedTypeIndex ) )
		{
			throw std::runtime_error{ "Could not find an appropriate memory type for buffer storage" };
		}

		VkMemoryAllocateInfo allocateInfo
		{
			VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
			nullptr,
			requirements.size,                        // allocationSize
			deducedTypeIndex                          // memoryTypeIndex
		};
		DEBUG_DUMP( allocateInfo );
		auto res = m_device.vkAllocateMemory( m_device, &allocateInfo, nullptr, &m_memory );
		checkError( res, "DeviceMemory allocation" );
	}

	DeviceMemory::~DeviceMemory()
	{
		m_device.vkFreeMemory( m_device, m_memory, nullptr );
	}

	uint8_t * DeviceMemory::lock( uint64_t offset
		, uint64_t size
		, ashes::MemoryMapFlags flags )const
	{
		uint8_t * pointer{ nullptr };
		auto res = m_device.vkMapMemory( m_device
			, m_memory
			, offset
			, size
			, 0u/*flags*/
			, reinterpret_cast< void ** >( &pointer ) );
		checkError( res, "DeviceMemory mapping" );
		return pointer;
	}

	void DeviceMemory::flush( uint64_t offset
		, uint64_t size )const
	{
		VkMappedMemoryRange mappedRange
		{
			VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE,
			nullptr,
			m_memory,                                         // memory
			0,                                                // offset
			size                                              // size
		};
		DEBUG_DUMP( mappedRange );
		auto res = m_device.vkFlushMappedMemoryRanges( m_device, 1, &mappedRange );
		checkError( res, "DeviceMemory range flush" );
	}

	void DeviceMemory::invalidate( uint64_t offset
		, uint64_t size )const
	{
		VkMappedMemoryRange mappedRange
		{
			VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE,
			nullptr,
			m_memory,                                         // memory
			0,                                                // offset
			size                                              // size
		};
		DEBUG_DUMP( mappedRange );
		auto res = m_device.vkInvalidateMappedMemoryRanges( m_device, 1, &mappedRange );
		checkError( res, "DeviceMemory mapped range invalidation" );
	}

	void DeviceMemory::unlock()const
	{
		m_device.vkUnmapMemory( m_device, m_memory );
	}
}
