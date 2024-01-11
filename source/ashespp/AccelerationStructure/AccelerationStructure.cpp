/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "ashespp/AccelerationStructure/AccelerationStructure.hpp"

#include "ashespp/Core/Device.hpp"
#include "ashespp/Miscellaneous/DeferredOperation.hpp"

namespace ashes
{
	AccelerationStructure::AccelerationStructure( Device const & device
		, VkAccelerationStructureCreateInfoKHR info )
		: AccelerationStructure{ device, "AccelerationStructure", std::move( info ) }
	{
	}

	AccelerationStructure::AccelerationStructure( Device const & device
		, std::string const & debugName
		, VkAccelerationStructureCreateInfoKHR info )
		: VkObject{ debugName }
		, m_device{ device }
		, m_info{ std::move( info ) }
	{
		auto res = m_device.vkCreateAccelerationStructureKHR( m_device
			, &m_info
			, m_device.getAllocationCallbacks()
			, &m_internal );
		checkError( res, "vkCreateAccelerationStructureKHR" );
		registerObject( m_device, debugName, *this );
	}

	AccelerationStructure::~AccelerationStructure()noexcept
	{
		unregisterObject( m_device, *this );
		m_device.vkDestroyAccelerationStructureKHR( m_device
			, m_internal
			, m_device.getAllocationCallbacks() );
	}

	void AccelerationStructure::copyAccelerationStructure( VkDeferredOperationKHR deferredOperation
		, AccelerationStructure const & dst
		, VkCopyAccelerationStructureModeKHR mode )const
	{
		VkCopyAccelerationStructureInfoKHR copy{ VK_STRUCTURE_TYPE_COPY_ACCELERATION_STRUCTURE_INFO_KHR
			, nullptr
			, m_internal
			, dst
			, mode };
		auto res = m_device.vkCopyAccelerationStructureKHR( m_device
			, deferredOperation
			, &copy );
		checkError( res, "vkCopyAccelerationStructureKHR" );
	}

	void AccelerationStructure::copyToMemory( VkDeferredOperationKHR deferredOperation
		, VkDeviceOrHostAddressKHR dst
		, VkCopyAccelerationStructureModeKHR mode )const
	{
		VkCopyAccelerationStructureToMemoryInfoKHR copy{ VK_STRUCTURE_TYPE_COPY_ACCELERATION_STRUCTURE_TO_MEMORY_INFO_KHR
			, nullptr
			, m_internal
			, dst
			, mode };
		auto res = m_device.vkCopyAccelerationStructureToMemoryKHR( m_device
			, deferredOperation
			, &copy );
		checkError( res, "vkCopyAccelerationStructureToMemoryKHR" );
	}

	void AccelerationStructure::copyFromMemory( VkDeferredOperationKHR deferredOperation
		, VkDeviceOrHostAddressConstKHR src
		, VkCopyAccelerationStructureModeKHR mode )const
	{
		VkCopyMemoryToAccelerationStructureInfoKHR copy{ VK_STRUCTURE_TYPE_COPY_MEMORY_TO_ACCELERATION_STRUCTURE_INFO_KHR
			, nullptr
			, src
			, m_internal
			, mode };
		auto res = m_device.vkCopyMemoryToAccelerationStructureKHR( m_device
			, deferredOperation
			, &copy );
		checkError( res, "vkCopyMemoryToAccelerationStructureKHR" );
	}

	VkDeviceAddress AccelerationStructure::getDeviceAddress()const
	{
		VkAccelerationStructureDeviceAddressInfoKHR info{ VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_DEVICE_ADDRESS_INFO_KHR
			, nullptr
			, m_internal };
		return m_device.vkGetAccelerationStructureDeviceAddressKHR( m_device
			, &info );
	}
}
