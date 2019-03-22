/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Core/VkPhysicalDevice.hpp"

#include "Core/VkInstance.hpp"
#include "Core/VkDevice.hpp"

#define VkRenderer_UsePhysicalDeviceExtensions 1

namespace vk_renderer
{
	PhysicalDevice::PhysicalDevice( Instance const & instance
		, VkPhysicalDevice gpu )
		: ashes::PhysicalDevice{ instance }
		, m_instance{ instance }
		, m_gpu{ gpu }
	{
		m_shaderVersion = 460u;
	}

	ashes::LayerPropertiesArray PhysicalDevice::enumerateLayerProperties()const
	{
		ashes::LayerPropertiesArray result;
		VkResult res;

		do
		{
			uint32_t layersCount = 0u;
			res = m_instance.vkEnumerateDeviceLayerProperties( m_gpu
				, &layersCount
				, NULL );

			if ( layersCount )
			{
				std::vector< VkLayerProperties > deviceLayerProperties;
				deviceLayerProperties.resize( layersCount );
				res = m_instance.vkEnumerateDeviceLayerProperties( m_gpu
					, &layersCount,
					deviceLayerProperties.data() );
				result = convert< ashes::LayerProperties >( deviceLayerProperties );
			}
		}
		while ( res == VK_INCOMPLETE );

		checkError( res, "Device layers retrieval" );
		return result;
	}

	ashes::ExtensionPropertiesArray PhysicalDevice::enumerateExtensionProperties( std::string const & layerName )const
	{
		ashes::ExtensionPropertiesArray result;
		VkResult res;

		do
		{
			uint32_t extensionsCount = 0;
			res = m_instance.vkEnumerateDeviceExtensionProperties( m_gpu
				, layerName.empty() ? nullptr : layerName.c_str()
				, &extensionsCount
				, nullptr );

			if ( extensionsCount )
			{
				std::vector< VkExtensionProperties > extensions;
				extensions.resize( extensionsCount );
				res = m_instance.vkEnumerateDeviceExtensionProperties( m_gpu
					, layerName.empty() ? nullptr : layerName.c_str()
					, &extensionsCount
					, extensions.data() );
				result = convert< ashes::ExtensionProperties >( extensions );
			}
		}
		while ( res == VK_INCOMPLETE );

		checkError( res, "Device layer extensions retrieval" );
		return result;
	}

	ashes::PhysicalDeviceProperties PhysicalDevice::getProperties()const
	{
		VkPhysicalDeviceProperties properties;
		m_instance.vkGetPhysicalDeviceProperties( m_gpu, &properties );
		return convert( properties );
	}

	ashes::PhysicalDeviceMemoryProperties PhysicalDevice::getMemoryProperties()const
	{
		VkPhysicalDeviceMemoryProperties memoryProperties;
		m_instance.vkGetPhysicalDeviceMemoryProperties( m_gpu, &memoryProperties );
		return convert( memoryProperties );
	}

	ashes::PhysicalDeviceFeatures PhysicalDevice::getFeatures()const
	{
		VkPhysicalDeviceFeatures features;
		m_instance.vkGetPhysicalDeviceFeatures( m_gpu, &features );
		return convert( features );
	}

	ashes::QueueFamilyPropertiesArray PhysicalDevice::getQueueFamilyProperties()const
	{
		uint32_t queueCount{ 0 };
		m_instance.vkGetPhysicalDeviceQueueFamilyProperties( m_gpu, &queueCount, nullptr );
		assert( queueCount >= 1 );

		std::vector< VkQueueFamilyProperties > queueProperties( queueCount );
		m_instance.vkGetPhysicalDeviceQueueFamilyProperties( m_gpu, &queueCount, queueProperties.data() );
		assert( queueCount >= 1 );

		return convert< ashes::QueueFamilyProperties >( queueProperties );
	}

	ashes::FormatProperties PhysicalDevice::getFormatProperties( ashes::Format fmt )const
	{
		VkFormatProperties props;
		m_instance.vkGetPhysicalDeviceFormatProperties( m_gpu, VkFormat( fmt ), &props );
		return convert( props );
	}
}
