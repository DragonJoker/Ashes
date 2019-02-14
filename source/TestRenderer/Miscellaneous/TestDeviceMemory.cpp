/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#include "Miscellaneous/TestDeviceMemory.hpp"

#include "Core/TestDevice.hpp"
#include "Core/TestPhysicalDevice.hpp"

namespace test_renderer
{
	DeviceMemory::DeviceMemory( Device const & device
		, ashes::MemoryRequirements const & requirements
		, ashes::MemoryPropertyFlags flags )
		: ashes::DeviceMemory{ device, flags }
		, m_memory( size_t( requirements.size ), 0 )
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
