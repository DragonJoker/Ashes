/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_DeviceCreateInfo_HPP___
#define ___AshesPP_DeviceCreateInfo_HPP___
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
			, enabledLayerNames{ std::move( enabledLayerNames ) }
			, enabledExtensionNames{ std::move( enabledExtensionNames ) }
			, ptrEnabledLayerNames{ convert( this->enabledLayerNames ) }
			, ptrEnabledExtensionNames{ convert( this->enabledExtensionNames ) }
			, vkQueueCreateInfos{ makeVkArray< VkDeviceQueueCreateInfo >( this->queueCreateInfos ) }
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
				&this->enabledFeatures,
			}
		{
		}

		DeviceCreateInfo( DeviceCreateInfo && rhs )
			: queueCreateInfos{ std::move( rhs.queueCreateInfos ) }
			, enabledLayerNames{ std::move( rhs.enabledLayerNames ) }
			, enabledExtensionNames{ std::move( rhs.enabledExtensionNames ) }
			, ptrEnabledLayerNames{ convert( this->enabledLayerNames ) }
			, ptrEnabledExtensionNames{ convert( this->enabledExtensionNames ) }
			, vkQueueCreateInfos{ makeVkArray< VkDeviceQueueCreateInfo >( this->queueCreateInfos ) }
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
				&this->enabledFeatures,
			}
		{
		}

		DeviceCreateInfo & operator=( DeviceCreateInfo && rhs )
		{
			queueCreateInfos = std::move( rhs.queueCreateInfos );
			enabledLayerNames = std::move( rhs.enabledLayerNames );
			enabledExtensionNames = std::move( rhs.enabledExtensionNames );
			ptrEnabledLayerNames = convert( this->enabledLayerNames );
			ptrEnabledExtensionNames = convert( this->enabledExtensionNames );
			vkQueueCreateInfos = makeVkArray< VkDeviceQueueCreateInfo >( this->queueCreateInfos );
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
				&this->enabledFeatures,
			};

			return *this;
		}

		inline operator VkDeviceCreateInfo const &()const
		{
			return vk;
		}

		DeviceQueueCreateInfoArray queueCreateInfos;
		StringArray enabledLayerNames;
		StringArray enabledExtensionNames;
		CharPtrArray ptrEnabledLayerNames;
		CharPtrArray ptrEnabledExtensionNames;
		VkPhysicalDeviceFeatures enabledFeatures;

	private:
		VkDeviceQueueCreateInfoArray vkQueueCreateInfos;
		VkDeviceCreateInfo vk;
	};
}

#endif
