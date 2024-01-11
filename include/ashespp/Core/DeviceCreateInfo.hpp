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
		~DeviceCreateInfo()noexcept = default;

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
			, vk{ VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO
				, nullptr
				, pflags
				, uint32_t( vkQueueCreateInfos.size() )
				, vkQueueCreateInfos.data()
				, uint32_t( ptrEnabledLayerNames.size() )
				, ptrEnabledLayerNames.data()
				, uint32_t( ptrEnabledExtensionNames.size() )
				, ptrEnabledExtensionNames.data()
				, enabledFeatures ? &enabledFeatures.value() : nullptr }
		{
		}

		DeviceCreateInfo( VkDeviceCreateFlags pflags
			, DeviceQueueCreateInfoArray pqueueCreateInfos
			, StringArray penabledLayerNames
			, StringArray penabledExtensionNames )
			: queueCreateInfos{ std::move( pqueueCreateInfos ) }
			, enabledLayerNames{ std::move( penabledLayerNames ) }
			, enabledExtensionNames{ std::move( penabledExtensionNames ) }
			, ptrEnabledLayerNames{ convert( enabledLayerNames ) }
			, ptrEnabledExtensionNames{ convert( enabledExtensionNames ) }
			, enabledFeatures{ nullopt }
			, vkQueueCreateInfos{ makeVkArray< VkDeviceQueueCreateInfo >( queueCreateInfos ) }
			, vk{ VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO
				, nullptr
				, pflags
				, uint32_t( vkQueueCreateInfos.size() )
				, vkQueueCreateInfos.data()
				, uint32_t( ptrEnabledLayerNames.size() )
				, ptrEnabledLayerNames.data()
				, uint32_t( ptrEnabledExtensionNames.size() )
				, ptrEnabledExtensionNames.data()
				, nullptr }
		{
		}

		explicit DeviceCreateInfo( VkDeviceCreateInfo const & rhs )noexcept
			: queueCreateInfos{ makeArray< DeviceQueueCreateInfo >( makeArray( rhs.pQueueCreateInfos, rhs.queueCreateInfoCount ) ) }
			, enabledLayerNames{ makeArray( rhs.ppEnabledLayerNames, rhs.enabledLayerCount ) }
			, enabledExtensionNames{ makeArray( rhs.ppEnabledExtensionNames, rhs.enabledExtensionCount ) }
			, ptrEnabledLayerNames{ convert( enabledLayerNames ) }
			, ptrEnabledExtensionNames{ convert( enabledExtensionNames ) }
			, enabledFeatures{ rhs.pEnabledFeatures ? Optional< VkPhysicalDeviceFeatures >( *rhs.pEnabledFeatures ) : nullopt }
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
				, enabledFeatures ? &enabledFeatures.value() : nullptr }
		{
		}

		DeviceCreateInfo( DeviceCreateInfo const & rhs )
			: queueCreateInfos{ rhs.queueCreateInfos }
			, enabledLayerNames{ rhs.enabledLayerNames }
			, enabledExtensionNames{ rhs.enabledExtensionNames }
			, ptrEnabledLayerNames{ convert( enabledLayerNames ) }
			, ptrEnabledExtensionNames{ convert( enabledExtensionNames ) }
			, enabledFeatures{ rhs.enabledFeatures }
			, vkQueueCreateInfos{ makeVkArray< VkDeviceQueueCreateInfo >( queueCreateInfos ) }
			, vk{ VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO
				, rhs.vk.pNext
				, rhs.vk.flags
				, uint32_t( vkQueueCreateInfos.size() )
				, vkQueueCreateInfos.data()
				, uint32_t( ptrEnabledLayerNames.size() )
				, ptrEnabledLayerNames.data()
				, uint32_t( ptrEnabledExtensionNames.size() )
				, ptrEnabledExtensionNames.data()
				, enabledFeatures ? &enabledFeatures.value() : nullptr }
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
			, vk{ VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO
			, rhs.vk.pNext
			, rhs.vk.flags
			, uint32_t( vkQueueCreateInfos.size() )
			, vkQueueCreateInfos.data()
			, uint32_t( ptrEnabledLayerNames.size() )
			, ptrEnabledLayerNames.data()
			, uint32_t( ptrEnabledExtensionNames.size() )
			, ptrEnabledExtensionNames.data()
			, enabledFeatures ? &enabledFeatures.value() : nullptr }
		{}

		DeviceCreateInfo & operator=( DeviceCreateInfo const & rhs )
		{
			queueCreateInfos = rhs.queueCreateInfos;
			enabledLayerNames = rhs.enabledLayerNames;
			enabledExtensionNames = rhs.enabledExtensionNames;
			ptrEnabledLayerNames = convert( enabledLayerNames );
			ptrEnabledExtensionNames = convert( enabledExtensionNames );
			enabledFeatures = rhs.enabledFeatures;
			vkQueueCreateInfos = makeVkArray< VkDeviceQueueCreateInfo >( queueCreateInfos );
			vk = { VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO
				, rhs.vk.pNext
				, rhs.vk.flags
				, uint32_t( vkQueueCreateInfos.size() )
				, vkQueueCreateInfos.data()
				, uint32_t( ptrEnabledLayerNames.size() )
				, ptrEnabledLayerNames.data()
				, uint32_t( ptrEnabledExtensionNames.size() )
				, ptrEnabledExtensionNames.data()
				, enabledFeatures ? &enabledFeatures.value() : nullptr };

			return *this;
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
			vk = { VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO
				, rhs.vk.pNext
				, rhs.vk.flags
				, uint32_t( vkQueueCreateInfos.size() )
				, vkQueueCreateInfos.data()
				, uint32_t( ptrEnabledLayerNames.size() )
				, ptrEnabledLayerNames.data()
				, uint32_t( ptrEnabledExtensionNames.size() )
				, ptrEnabledExtensionNames.data()
				, enabledFeatures ? &enabledFeatures.value() : nullptr };

			return *this;
		}

		operator VkDeviceCreateInfo const &()const noexcept
		{
			return vk;
		}

		VkDeviceCreateInfo const * operator->()const noexcept
		{
			return &vk;
		}

		VkDeviceCreateInfo * operator->()noexcept
		{
			return &vk;
		}

		DeviceQueueCreateInfoArray queueCreateInfos;
		StringArray enabledLayerNames;
		StringArray enabledExtensionNames;
		CharPtrArray ptrEnabledLayerNames;
		CharPtrArray ptrEnabledExtensionNames;
		Optional< VkPhysicalDeviceFeatures > enabledFeatures;

	private:
		VkDeviceQueueCreateInfoArray vkQueueCreateInfos;
		VkDeviceCreateInfo vk;
	};
}

#endif
