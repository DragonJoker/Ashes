/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#include "Core/VkPhysicalDevice.hpp"

#include "Core/VkRenderer.hpp"
#include "Core/VkDevice.hpp"

namespace vk_renderer
{
	namespace
	{
		std::string getName( VkPhysicalDeviceType type )
		{
			switch ( type )
			{
#define STR(r) case VK_PHYSICAL_DEVICE_TYPE_ ##r: return #r
				STR( OTHER );
				STR( INTEGRATED_GPU );
				STR( DISCRETE_GPU );
				STR( VIRTUAL_GPU );
#undef STR
			default: return "UNKNOWN_DEVICE_TYPE";
			}
		}

	}
	PhysicalDevice::PhysicalDevice( Renderer & renderer
		, VkPhysicalDevice gpu )
		: m_gpu{ gpu }
		, m_renderer{ renderer }
	{
		// On récupère les extensions supportées par le GPU.
		uint32_t extensionCount{ 0 };
		auto res = m_renderer.vkEnumerateDeviceExtensionProperties( m_gpu
			, nullptr
			, &extensionCount
			, nullptr );

		if ( !checkError( res ) )
		{
			throw std::runtime_error{ "GPU's extensions enumeration failed: " + getLastError() };
		}

		std::vector< VkExtensionProperties > extensions( extensionCount );
		res = m_renderer.vkEnumerateDeviceExtensionProperties( m_gpu
			, nullptr
			, &extensionCount
			, extensions.data() );

		if ( !checkError( res ) )
		{
			throw std::runtime_error{ "GPU's extensions enumeration failed: " + getLastError() };
		}

		m_renderer.completeLayerNames( m_deviceLayerNames );
		m_deviceExtensionNames.push_back( VK_KHR_SWAPCHAIN_EXTENSION_NAME );
		checkExtensionsAvailability( extensions, m_deviceExtensionNames );

		// Puis les capacités du GPU.
		m_renderer.vkGetPhysicalDeviceMemoryProperties( m_gpu, &m_memoryProperties );
		m_renderer.vkGetPhysicalDeviceProperties( m_gpu, &m_properties );
		m_renderer.vkGetPhysicalDeviceFeatures( m_gpu, &m_features );

		m_info.name = m_properties.deviceName;
		m_info.type = getName( m_properties.deviceType );
		std::stringstream api;
		api << ( m_properties.apiVersion >> 22 ) << "." << ( ( m_properties.apiVersion >> 12 ) & 0x3ff ) << "." << ( m_properties.apiVersion & 0xfff );
		m_info.apiVersion = api.str();
		m_info.fillModeNonSolid = m_features.fillModeNonSolid;
		m_info.wideLines = m_features.wideLines;
		m_info.textureCompressionASTC_LDR = m_features.textureCompressionASTC_LDR;
		m_info.textureCompressionBC = m_features.textureCompressionBC;
		m_info.textureCompressionETC2 = m_features.textureCompressionETC2;

		// Et enfin les propriétés des familles de files du GPU.
		uint32_t queueCount{ 0 };
		m_renderer.vkGetPhysicalDeviceQueueFamilyProperties( m_gpu, &queueCount, nullptr );
		assert( queueCount >= 1 );

		m_queueProperties.resize( queueCount );
		m_renderer.vkGetPhysicalDeviceQueueFamilyProperties( m_gpu, &queueCount, m_queueProperties.data() );
		assert( queueCount >= 1 );
	}

	bool PhysicalDevice::deduceMemoryType( uint32_t typeBits
		, VkMemoryPropertyFlags requirements
		, uint32_t & typeIndex )const
	{
		bool result{ false };

		// Recherche parmi les types de mémoire de la première ayant les propriétés voulues.
		uint32_t i{ 0 };
		while ( i < m_memoryProperties.memoryTypeCount && !result )
		{
			if ( ( typeBits & 1 ) == 1 )
			{
				// Le type de mémoire est disponible, a-t-il les propriétés demandées?
				if ( ( m_memoryProperties.memoryTypes[i].propertyFlags & requirements ) == requirements )
				{
					typeIndex = i;
					result = true;
				}
			}

			typeBits >>= 1;
			++i;
		}

		return result;
	}
}
