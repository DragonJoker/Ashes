/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_DeviceCreateInfo_HPP___
#define ___Ashes_DeviceCreateInfo_HPP___
#pragma once

#include "DeviceQueueCreateInfo.hpp"

namespace ashes
{
	struct DeviceCreateInfo
	{
		DeviceCreateInfo( DeviceCreateInfo const & ) = delete;
		DeviceCreateInfo & operator=( DeviceCreateInfo const & ) = delete;

		DeviceCreateInfo( VkDeviceCreateFlags flags
			, DeviceQueueCreateInfoArray queueCreateInfos
			, StringArray enabledLayerNames
			, StringArray enabledExtensionNames
			, VkPhysicalDeviceFeatures enabledFeatures )
			: queueCreateInfos{ std::move( queueCreateInfos ) }
			, vkQueueCreateInfos{ makeVkArray< VkDeviceQueueCreateInfo >( this->queueCreateInfos ) }
			, enabledLayerNames{ std::move( enabledLayerNames ) }
			, enabledExtensionNames{ std::move( enabledExtensionNames ) }
			, ptrEnabledLayerNames{ convert( this->enabledLayerNames ) }
			, ptrEnabledExtensionNames{ convert( this->enabledExtensionNames ) }
			, enabledFeatures{ std::move( enabledFeatures ) }
			, vk
			{
				VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
				nullptr,
				flags,
				uint32_t( this->vkQueueCreateInfos.size() ),
				this->vkQueueCreateInfos.data(),
				uint32_t( this->ptrEnabledLayerNames.size() ),
				this->ptrEnabledLayerNames.data(),
				uint32_t( this->ptrEnabledExtensionNames.size() ),
				this->ptrEnabledExtensionNames.data(),
				&enabledFeatures,
			}
		{
		}

		DeviceCreateInfo( DeviceCreateInfo && rhs )
			: queueCreateInfos{ std::move( rhs.queueCreateInfos ) }
			, vkQueueCreateInfos{ makeVkArray< VkDeviceQueueCreateInfo >( this->queueCreateInfos ) }
			, enabledLayerNames{ std::move( rhs.enabledLayerNames ) }
			, enabledExtensionNames{ std::move( rhs.enabledExtensionNames ) }
			, ptrEnabledLayerNames{ convert( this->enabledLayerNames ) }
			, ptrEnabledExtensionNames{ convert( this->enabledExtensionNames ) }
			, enabledFeatures{ std::move( rhs.enabledFeatures ) }
			, vk
			{
				VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
				nullptr,
				rhs.vk.flags,
				uint32_t( this->vkQueueCreateInfos.size() ),
				this->vkQueueCreateInfos.data(),
				uint32_t( this->ptrEnabledLayerNames.size() ),
				this->ptrEnabledLayerNames.data(),
				uint32_t( this->ptrEnabledExtensionNames.size() ),
				this->ptrEnabledExtensionNames.data(),
				&enabledFeatures,
			}
		{
		}

		DeviceCreateInfo & operator=( DeviceCreateInfo && rhs )
		{
			queueCreateInfos = std::move( rhs.queueCreateInfos );
			vkQueueCreateInfos = makeVkArray< VkDeviceQueueCreateInfo >( this->queueCreateInfos );
			enabledLayerNames = std::move( rhs.enabledLayerNames );
			enabledExtensionNames = std::move( rhs.enabledExtensionNames );
			ptrEnabledLayerNames = convert( this->enabledLayerNames );
			ptrEnabledExtensionNames = convert( this->enabledExtensionNames );
			enabledFeatures = std::move( rhs.enabledFeatures );
			vk =
			{
				VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
				nullptr,
				rhs.vk.flags,
				uint32_t( this->vkQueueCreateInfos.size() ),
				this->vkQueueCreateInfos.data(),
				uint32_t( this->ptrEnabledLayerNames.size() ),
				this->ptrEnabledLayerNames.data(),
				uint32_t( this->ptrEnabledExtensionNames.size() ),
				this->ptrEnabledExtensionNames.data(),
				&enabledFeatures,
			};

			return *this;
		}

		inline operator VkDeviceCreateInfo const &()const
		{
			return vk;
		}

	private:
		DeviceQueueCreateInfoArray queueCreateInfos;
		VkDeviceQueueCreateInfoArray vkQueueCreateInfos;
		StringArray enabledLayerNames;
		StringArray enabledExtensionNames;
		CharPtrArray ptrEnabledLayerNames;
		CharPtrArray ptrEnabledExtensionNames;
		VkPhysicalDeviceFeatures enabledFeatures;
		VkDeviceCreateInfo vk;
	};
}

#endif
