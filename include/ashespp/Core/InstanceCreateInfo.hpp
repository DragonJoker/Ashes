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

		InstanceCreateInfo( VkDeviceCreateFlags flags
			, ApplicationInfo applicationInfo
			, StringArray enabledLayerNames
			, StringArray enabledExtensionNames )
			: applicationInfo{ std::move( applicationInfo ) }
			, enabledLayerNames{ std::move( enabledLayerNames ) }
			, enabledExtensionNames{ std::move( enabledExtensionNames ) }
			, ptrEnabledLayerNames{ convert( this->enabledLayerNames ) }
			, ptrEnabledExtensionNames{ convert( this->enabledExtensionNames ) }
			, vk
			{
				VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
				nullptr,
				flags,
				&static_cast< VkApplicationInfo const & >( this->applicationInfo ),
				uint32_t( this->ptrEnabledLayerNames.size() ),
				this->ptrEnabledLayerNames.data(),
				uint32_t( this->ptrEnabledExtensionNames.size() ),
				this->ptrEnabledExtensionNames.data(),
			}
		{
		}
		
		InstanceCreateInfo( InstanceCreateInfo && rhs )noexcept
			: applicationInfo{ std::move( rhs.applicationInfo ) }
			, enabledLayerNames{ std::move( rhs.enabledLayerNames ) }
			, enabledExtensionNames{ std::move( rhs.enabledExtensionNames ) }
			, ptrEnabledLayerNames{ convert( this->enabledLayerNames ) }
			, ptrEnabledExtensionNames{ convert( this->enabledExtensionNames ) }
			, vk
			{
				VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
				nullptr,
				rhs.vk.flags,
				&static_cast< VkApplicationInfo const & >( this->applicationInfo ),
				uint32_t( this->ptrEnabledLayerNames.size() ),
				this->ptrEnabledLayerNames.data(),
				uint32_t( this->ptrEnabledExtensionNames.size() ),
				this->ptrEnabledExtensionNames.data(),
			}
		{
		}
		
		InstanceCreateInfo & operator=( InstanceCreateInfo && rhs )noexcept
		{
			applicationInfo = std::move( rhs.applicationInfo );
			enabledLayerNames = std::move( rhs.enabledLayerNames );
			enabledExtensionNames = std::move( rhs.enabledExtensionNames );
			enabledFeatures = std::move( rhs.enabledFeatures );
			ptrEnabledLayerNames = convert( this->enabledLayerNames );
			ptrEnabledExtensionNames = convert( this->enabledExtensionNames );
			vk =
			{
				VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
				nullptr,
				rhs.vk.flags,
				&static_cast< VkApplicationInfo const & >( this->applicationInfo ),
				uint32_t( this->ptrEnabledLayerNames.size() ),
				this->ptrEnabledLayerNames.data(),
				uint32_t( this->ptrEnabledExtensionNames.size() ),
				this->ptrEnabledExtensionNames.data(),
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
