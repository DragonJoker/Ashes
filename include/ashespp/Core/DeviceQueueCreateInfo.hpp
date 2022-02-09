/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_DeviceQueueCreateInfo_HPP___
#define ___AshesPP_DeviceQueueCreateInfo_HPP___
#pragma once

#include "ashespp/AshesPPPrerequisites.hpp"

namespace ashes
{
	struct DeviceQueueCreateInfo
	{
		DeviceQueueCreateInfo( DeviceQueueCreateInfo const & ) = delete;
		DeviceQueueCreateInfo& operator=( DeviceQueueCreateInfo  const & ) = delete;

		DeviceQueueCreateInfo( VkDeviceQueueCreateFlags pflags
			, uint32_t pqueueFamilyIndex
			, FloatArray pqueuePriorities )
			: queuePriorities{ std::move( pqueuePriorities ) }
			, vk{ VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO
				, nullptr
				, pflags
				, pqueueFamilyIndex
				, uint32_t( queuePriorities.size() )
				, queuePriorities.data() }
		{
		}
		
		DeviceQueueCreateInfo( DeviceQueueCreateInfo && rhs )noexcept
			: queuePriorities{ std::move( rhs.queuePriorities ) }
			, vk{ VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO
				, rhs.vk.pNext
				, rhs.vk.flags
				, rhs.vk.queueFamilyIndex
				, uint32_t( queuePriorities.size() )
				, queuePriorities.data() }
		{
		}
		
		DeviceQueueCreateInfo( VkDeviceQueueCreateInfo rhs )noexcept
			: queuePriorities{ makeArray( rhs.pQueuePriorities, rhs.queueCount ) }
			, vk{ VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO
				, rhs.pNext
				, rhs.flags
				, rhs.queueFamilyIndex
				, uint32_t( queuePriorities.size() )
				, queuePriorities.data() }
		{
		}
		
		DeviceQueueCreateInfo & operator=( DeviceQueueCreateInfo && rhs )noexcept
		{
			queuePriorities = std::move( rhs.queuePriorities );
			vk = { VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO
				, rhs.vk.pNext
				, rhs.vk.flags
				, rhs.vk.queueFamilyIndex
				, uint32_t( queuePriorities.size() )
				, queuePriorities.data() };
			return *this;
		}

		operator VkDeviceQueueCreateInfo const &()const
		{
			return vk;
		}

		VkDeviceQueueCreateInfo const * operator->()const
		{
			return &vk;
		}

		VkDeviceQueueCreateInfo * operator->()
		{
			return &vk;
		}

	private:
		FloatArray queuePriorities;
		VkDeviceQueueCreateInfo vk;
	};
	using DeviceQueueCreateInfoArray = std::vector< DeviceQueueCreateInfo >;

	inline DeviceQueueCreateInfo convert( VkDeviceQueueCreateInfo const & vk )
	{
		return DeviceQueueCreateInfo{ vk };
	}

	template< typename Type >
	std::vector< Type > makeArray( std::vector< VkDeviceQueueCreateInfo > const & values )
	{
		std::vector< Type > result;
		result.reserve( values.size() );

		for ( auto & value : values )
		{
			result.emplace_back( value );
		}

		return result;
	}
}

#endif
