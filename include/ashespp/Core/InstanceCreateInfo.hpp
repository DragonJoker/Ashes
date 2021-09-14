/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_InstanceCreateInfo_HPP___
#define ___AshesPP_InstanceCreateInfo_HPP___
#pragma once

#include "ApplicationInfo.hpp"

namespace ashes
{
	struct InstanceCreateInfo
	{
		InstanceCreateInfo( InstanceCreateInfo const & ) = delete;
		InstanceCreateInfo & operator=( InstanceCreateInfo const & ) = delete;

		InstanceCreateInfo( VkDeviceCreateFlags pflags
			, ApplicationInfo papplicationInfo
			, StringArray penabledLayerNames
			, StringArray penabledExtensionNames )
			: applicationInfo{ std::move( papplicationInfo ) }
			, enabledLayerNames{ std::move( penabledLayerNames ) }
			, enabledExtensionNames{ std::move( penabledExtensionNames ) }
			, ptrEnabledLayerNames{ convert( enabledLayerNames ) }
			, ptrEnabledExtensionNames{ convert( enabledExtensionNames ) }
			, vk
			{
				VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
				nullptr,
				pflags,
				&static_cast< VkApplicationInfo const & >( applicationInfo ),
				uint32_t( ptrEnabledLayerNames.size() ),
				ptrEnabledLayerNames.data(),
				uint32_t( ptrEnabledExtensionNames.size() ),
				ptrEnabledExtensionNames.data(),
			}
		{
		}
		
		InstanceCreateInfo( InstanceCreateInfo && rhs )noexcept
			: applicationInfo{ std::move( rhs.applicationInfo ) }
			, enabledLayerNames{ std::move( rhs.enabledLayerNames ) }
			, enabledExtensionNames{ std::move( rhs.enabledExtensionNames ) }
			, ptrEnabledLayerNames{ convert( enabledLayerNames ) }
			, ptrEnabledExtensionNames{ convert( enabledExtensionNames ) }
			, vk
			{
				VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
				nullptr,
				rhs.vk.flags,
				&static_cast< VkApplicationInfo const & >( applicationInfo ),
				uint32_t( ptrEnabledLayerNames.size() ),
				ptrEnabledLayerNames.data(),
				uint32_t( ptrEnabledExtensionNames.size() ),
				ptrEnabledExtensionNames.data(),
			}
		{
		}
		
		InstanceCreateInfo & operator=( InstanceCreateInfo && rhs )noexcept
		{
			applicationInfo = std::move( rhs.applicationInfo );
			enabledLayerNames = std::move( rhs.enabledLayerNames );
			enabledExtensionNames = std::move( rhs.enabledExtensionNames );
			enabledFeatures = std::move( rhs.enabledFeatures );
			ptrEnabledLayerNames = convert( enabledLayerNames );
			ptrEnabledExtensionNames = convert( enabledExtensionNames );
			vk =
			{
				VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
				nullptr,
				rhs.vk.flags,
				&static_cast< VkApplicationInfo const & >( applicationInfo ),
				uint32_t( ptrEnabledLayerNames.size() ),
				ptrEnabledLayerNames.data(),
				uint32_t( ptrEnabledExtensionNames.size() ),
				ptrEnabledExtensionNames.data(),
			};

			return *this;
		}

		inline operator VkInstanceCreateInfo const &()const
		{
			return vk;
		}

		ApplicationInfo applicationInfo;
		StringArray enabledLayerNames;
		StringArray enabledExtensionNames;
		CharPtrArray ptrEnabledLayerNames;
		CharPtrArray ptrEnabledExtensionNames;
		VkPhysicalDeviceFeatures enabledFeatures{};

	private:
		VkInstanceCreateInfo vk;
	};
}

#endif
