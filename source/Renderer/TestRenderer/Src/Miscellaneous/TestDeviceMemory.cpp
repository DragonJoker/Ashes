/*
This file belongs to RendererLib.
See LICENSE file in root folder
*/
#include "Miscellaneous/TestDeviceMemory.hpp"

#include "Core/TestDevice.hpp"
#include "Core/TestPhysicalDevice.hpp"

namespace test_renderer
{
	DeviceMemory::DeviceMemory( Device const & device
		, renderer::MemoryRequirements const & requirements
		, renderer::MemoryPropertyFlags flags )
		: renderer::DeviceMemory{ device, flags }
		, m_memory( requirements.size, 0 )
	{
	}

	DeviceMemory::~DeviceMemory()
	{
	}

	uint8_t * DeviceMemory::lock( uint32_t offset
		, uint32_t size
		, renderer::MemoryMapFlags flags )const
	{
		return m_memory.data() + offset;
	}

	void DeviceMemory::flush( uint32_t offset
		, uint32_t size )const
	{
	}

	void DeviceMemory::invalidate( uint32_t offset
		, uint32_t size )const
	{
	}

	void DeviceMemory::unlock()const
	{
	}
}
