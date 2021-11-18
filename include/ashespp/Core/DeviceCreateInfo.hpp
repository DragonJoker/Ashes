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

		DeviceCreateInfo( VkDeviceCreateFlags pflags
			, DeviceQueueCreateInfoArray pqueueCreateInfos
			, StringArray penabledLayerNames
			, StringArray penabledExtensionNames
			, VkPhysicalDeviceFeatures penabledFeatures )
			: queueCreateInfos{ std::move( pqueueCreateInfos ) }
			, enabledLayerNames{ std::move( penabledLayerNames ) }
			, enabledExtensionNames{ std::move( penabledExtensionNames ) }
			, ptrEnabledLayerNames{ convert( enabledLayerNames ) }
			, ptrEnabledExtensionNames{ convert( enabledExtensionNames ) }
			, enabledFeatures{ std::move( penabledFeatures ) }
			, vkQueueCreateInfos{ makeVkArray< VkDeviceQueueCreateInfo >( queueCreateInfos ) }
			, vk
			{
				VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
				nullptr,
				pflags,
				uint32_t( vkQueueCreateInfos.size() ),
				vkQueueCreateInfos.data(),
				uint32_t( ptrEnabledLayerNames.size() ),
				ptrEnabledLayerNames.data(),
				uint32_t( ptrEnabledExtensionNames.size() ),
				ptrEnabledExtensionNames.data(),
				&enabledFeatures,
			}
		{
		}

		DeviceCreateInfo( VkDeviceCreateInfo rhs )noexcept
			: queueCreateInfos{ makeArray< DeviceQueueCreateInfo >( makeArray( rhs.pQueueCreateInfos, rhs.queueCreateInfoCount ) ) }
			, enabledLayerNames{ makeArray( rhs.ppEnabledLayerNames, rhs.enabledLayerCount ) }
			, enabledExtensionNames{ makeArray( rhs.ppEnabledExtensionNames, rhs.enabledExtensionCount ) }
			, ptrEnabledLayerNames{ convert( enabledLayerNames ) }
			, ptrEnabledExtensionNames{ convert( enabledExtensionNames ) }
			, enabledFeatures{ rhs.pEnabledFeatures ? *rhs.pEnabledFeatures : VkPhysicalDeviceFeatures{} }
			, vkQueueCreateInfos{ makeVkArray< VkDeviceQueueCreateInfo >( queueCreateInfos ) }
			, vk{ rhs.sType
				, rhs.pNext
				, rhs.flags
				, uint32_t( vkQueueCreateInfos.size() )
				, vkQueueCreateInfos.data()
				, uint32_t( ptrEnabledLayerNames.size() )
				, ptrEnabledLayerNames.data()
				, uint32_t( ptrEnabledExtensionNames.size() )
				, ptrEnabledExtensionNames.data()
				, ( rhs.pEnabledFeatures ? &enabledFeatures : nullptr ) }
		{
		}

		DeviceCreateInfo( DeviceCreateInfo && rhs )noexcept
			: queueCreateInfos{ std::move( rhs.queueCreateInfos ) }
			, enabledLayerNames{ std::move( rhs.enabledLayerNames ) }
			, enabledExtensionNames{ std::move( rhs.enabledExtensionNames ) }
			, ptrEnabledLayerNames{ convert( enabledLayerNames ) }
			, ptrEnabledExtensionNames{ convert( enabledExtensionNames ) }
			, enabledFeatures{ std::move( rhs.enabledFeatures ) }
			, vkQueueCreateInfos{ makeVkArray< VkDeviceQueueCreateInfo >( queueCreateInfos ) }
			, vk
			{
				VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
				nullptr,
				rhs.vk.flags,
				uint32_t( vkQueueCreateInfos.size() ),
				vkQueueCreateInfos.data(),
				uint32_t( ptrEnabledLayerNames.size() ),
				ptrEnabledLayerNames.data(),
				uint32_t( ptrEnabledExtensionNames.size() ),
				ptrEnabledExtensionNames.data(),
				&enabledFeatures,
			}
		{
		}

		DeviceCreateInfo & operator=( DeviceCreateInfo && rhs )noexcept
		{
			queueCreateInfos = std::move( rhs.queueCreateInfos );
			enabledLayerNames = std::move( rhs.enabledLayerNames );
			enabledExtensionNames = std::move( rhs.enabledExtensionNames );
			ptrEnabledLayerNames = convert( enabledLayerNames );
			ptrEnabledExtensionNames = convert( enabledExtensionNames );
			enabledFeatures = std::move( rhs.enabledFeatures );
			vkQueueCreateInfos = makeVkArray< VkDeviceQueueCreateInfo >( queueCreateInfos );
			vk =
			{
				VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
				nullptr,
				rhs.vk.flags,
				uint32_t( vkQueueCreateInfos.size() ),
				vkQueueCreateInfos.data(),
				uint32_t( ptrEnabledLayerNames.size() ),
				ptrEnabledLayerNames.data(),
				uint32_t( ptrEnabledExtensionNames.size() ),
				ptrEnabledExtensionNames.data(),
				&enabledFeatures,
			};

			return *this;
		}

		VkDeviceCreateInfo * operator->()
		{
			return &vk;
		}

		operator VkDeviceCreateInfo const &()const
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
