/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#include "ashespp/Miscellaneous/DeviceMemory.hpp"

#include "ashespp/Core/Device.hpp"
#include "ashespp/Core/PhysicalDevice.hpp"

namespace ashes
{
	DeviceMemory::DeviceMemory( Device const & device
		, VkMemoryAllocateInfo allocateInfo )
		: DeviceMemory{ device, "DeviceMemory", allocateInfo }
	{
	}

	DeviceMemory::DeviceMemory( Device const & device
		, std::string const & debugName
		, VkMemoryAllocateInfo allocateInfo )
		: VkObject{ debugName }
		, m_device{ device }
		, m_allocateInfo{ std::move( allocateInfo ) }
	{
		DEBUG_DUMP( m_allocateInfo );
		auto res = m_device.vkAllocateMemory( m_device
			, &m_allocateInfo
			, m_device.getAllocationCallbacks()
			, &m_internal );
		checkError( res, "DeviceMemory allocation" );
		registerObject( m_device, debugName, *this );
	}

	DeviceMemory::~DeviceMemory()
	{
		unregisterObject( m_device, *this );
		m_device.vkFreeMemory( m_device
			, m_internal
			, m_device.getAllocationCallbacks() );
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
			offset,                                           // offset
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
			offset,                                           // offset
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
