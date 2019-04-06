/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_DeviceQueueCreateInfo_HPP___
#define ___Ashes_DeviceQueueCreateInfo_HPP___
#pragma once

#include "AshesRenderer/AshesRendererPrerequisites.hpp"

namespace ashes
{
	struct DeviceQueueCreateInfo
	{
		DeviceQueueCreateInfo( DeviceQueueCreateInfo const & ) = delete;
		DeviceQueueCreateInfo& operator=( DeviceQueueCreateInfo  const & ) = delete;

		DeviceQueueCreateInfo( VkDeviceQueueCreateFlags flags
			, uint32_t queueFamilyIndex
			, FloatArray queuePriorities )
			: queuePriorities{ std::move( queuePriorities ) }
			, vk
			{
				VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
				nullptr,
				flags,
				queueFamilyIndex,
				uint32_t( this->queuePriorities.size() ),
				this->queuePriorities.data(),
			}
		{
		}
		
		DeviceQueueCreateInfo( DeviceQueueCreateInfo && rhs )
			: queuePriorities{ std::move( rhs.queuePriorities ) }
			, vk
			{
				VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
				nullptr,
				rhs.vk.flags,
				rhs.vk.queueFamilyIndex,
				uint32_t( this->queuePriorities.size() ),
				this->queuePriorities.data(),
			}
		{
		}
		
		DeviceQueueCreateInfo & operator=( DeviceQueueCreateInfo && rhs )
		{
			queuePriorities = std::move( rhs.queuePriorities );
			vk =
			{
				VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
				nullptr,
				rhs.vk.flags,
				rhs.vk.queueFamilyIndex,
				uint32_t( this->queuePriorities.size() ),
				this->queuePriorities.data(),
			};

			return *this;
		}

		inline operator VkDeviceQueueCreateInfo const &()const
		{
			return vk;
		}

	private:
		FloatArray queuePriorities;
		VkDeviceQueueCreateInfo vk;
	};
	using DeviceQueueCreateInfoArray = std::vector< DeviceQueueCreateInfo >;
}

#endif
