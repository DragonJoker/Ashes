/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#include "Miscellaneous/TestDeviceMemory.hpp"

#include "Core/TestDevice.hpp"
#include "Core/TestInstance.hpp"
#include "Core/TestPhysicalDevice.hpp"

namespace test_renderer
{
	//*********************************************************************************************

	ashes::MemoryPropertyFlags getFlags( uint32_t memoryTypeIndex )
	{
		assert( memoryTypeIndex < Instance::getMemoryProperties().memoryTypes.size()
			&& "Wrong deduced memory type" );
		return Instance::getMemoryProperties().memoryTypes[memoryTypeIndex].propertyFlags;
	}

	//*********************************************************************************************

	DeviceMemory::DeviceMemory( Device const & device
		, ashes::MemoryAllocateInfo allocateInfo )
		: ashes::DeviceMemory{ device, std::move( allocateInfo ) }
		, m_memory( size_t( m_allocateInfo.allocationSize ), 0 )
	{
	}

	DeviceMemory::~DeviceMemory()
	{
	}

	uint8_t * DeviceMemory::lock( uint64_t offset
		, uint64_t size
		, ashes::MemoryMapFlags flags )const
	{
		return m_memory.data() + offset;
	}

	void DeviceMemory::flush( uint64_t offset
		, uint64_t size )const
	{
	}

	void DeviceMemory::invalidate( uint64_t offset
		, uint64_t size )const
	{
	}

	void DeviceMemory::unlock()const
	{
	}
}
