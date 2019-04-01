/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#include "AshesPP/Miscellaneous/DeviceMemory.hpp"

#include "AshesPP/Core/Device.hpp"
#include "AshesPP/Core/PhysicalDevice.hpp"

namespace ashespp
{
	DeviceMemory::DeviceMemory( Device const & device
		, VkMemoryAllocateInfo allocateInfo )
		: m_device{ device }
		, m_allocateInfo{ std::move( allocateInfo ) }
	{
		DEBUG_DUMP( m_allocateInfo );
		auto res = m_device.vkAllocateMemory( m_device
			, &m_allocateInfo
			, nullptr
			, &m_internal );
		checkError( res, "DeviceMemory allocation" );
		registerObject( m_device, "DeviceMemory", this );
	}

	DeviceMemory::~DeviceMemory()
	{
		unregisterObject( m_device, this );
		m_device.vkFreeMemory( m_device, m_internal, nullptr );
	}

	uint8_t * DeviceMemory::lock( VkDeviceSize offset
		, VkDeviceSize size
		, VkMemoryMapFlags flags )const
	{
		uint8_t * pointer{ nullptr };
		auto res = m_device.vkMapMemory( m_device
			, m_internal
			, offset
			, size
			, 0u/*flags*/
			, reinterpret_cast< void ** >( &pointer ) );
		checkError( res, "DeviceMemory mapping" );
		return pointer;
	}

	void DeviceMemory::flush( VkDeviceSize offset
		, VkDeviceSize size )const
	{
		VkMappedMemoryRange mappedRange
		{
			VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE,
			nullptr,
			m_internal,                                       // memory
			0,                                                // offset
			size                                              // size
		};
		DEBUG_DUMP( mappedRange );
		auto res = m_device.vkFlushMappedMemoryRanges( m_device, 1, &mappedRange );
		checkError( res, "DeviceMemory range flush" );
	}

	void DeviceMemory::invalidate( VkDeviceSize offset
		, VkDeviceSize size )const
	{
		VkMappedMemoryRange mappedRange
		{
			VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE,
			nullptr,
			m_internal,                                       // memory
			0,                                                // offset
			size                                              // size
		};
		DEBUG_DUMP( mappedRange );
		auto res = m_device.vkInvalidateMappedMemoryRanges( m_device, 1, &mappedRange );
		checkError( res, "DeviceMemory mapped range invalidation" );
	}

	void DeviceMemory::unlock()const
	{
		m_device.vkUnmapMemory( m_device, m_internal );
	}
}
