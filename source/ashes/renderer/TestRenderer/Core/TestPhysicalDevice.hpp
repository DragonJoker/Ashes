/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/TestRenderer/TestRendererPrerequisites.hpp"

#include <renderer/RendererCommon/IcdObject.hpp>

namespace ashes::test
{
	class PhysicalDevice
		: public ashes::IcdObject
	{
	public:
		explicit PhysicalDevice( VkInstance instance );

		VkBool32 getPresentationSupport()const;
		VkLayerPropertiesArray const & enumerateLayerProperties()const noexcept;
		VkExtensionPropertiesArray const & enumerateExtensionProperties()const noexcept;
		VkPhysicalDeviceProperties const & getProperties()const;
		VkPhysicalDeviceMemoryProperties getMemoryProperties()const;
		VkPhysicalDeviceFeatures getFeatures()const;
		VkQueueFamilyPropertiesArray getQueueFamilyProperties()const;
		VkFormatProperties getFormatProperties( VkFormat fmt )const;
		VkResult getImageFormatProperties()const;
		VkResult getSparseImageFormatProperties()const;

#ifdef VK_VERSION_1_1
		VkPhysicalDeviceFeatures2 const & getFeatures2()const;
		VkPhysicalDeviceProperties2 const & getProperties2()const;
		VkFormatProperties2 const & getFormatProperties2( VkFormat format )const;
		VkResult getImageFormatProperties2( VkPhysicalDeviceImageFormatInfo2 const & imageFormatInfo
			, VkImageFormatProperties2 & imageProperties )const;
		std::vector< VkQueueFamilyProperties2 > getQueueFamilyProperties2()const;
		VkPhysicalDeviceMemoryProperties2 const & getMemoryProperties2()const;
		VkResult getSparseImageFormatProperties2( VkPhysicalDeviceSparseImageFormatInfo2 const & formatInfo
			, std::vector< VkSparseImageFormatProperties2 > & sparseImageFormatProperties )const;
#elif VK_KHR_get_physical_device_properties2
		VkPhysicalDeviceFeatures2KHR const & getFeatures2()const;
		VkPhysicalDeviceProperties2KHR const & getProperties2()const;
		VkFormatProperties2KHR const & getFormatProperties2( VkFormat format )const;
		VkResult getImageFormatProperties2( VkPhysicalDeviceImageFormatInfo2KHR const & imageFormatInfo
			, VkImageFormatProperties2KHR & imageProperties )const;
		std::vector< VkQueueFamilyProperties2KHR > getQueueFamilyProperties2()const;
		VkPhysicalDeviceMemoryProperties2KHR const & getMemoryProperties2()const;
		VkResult getSparseImageFormatProperties2( VkPhysicalDeviceSparseImageFormatInfo2KHR const & formatInfo
			, std::vector< VkSparseImageFormatProperties2KHR > & sparseImageFormatProperties )const;
#endif
#ifdef VK_KHR_display
		std::vector< VkDisplayPropertiesKHR > const & getDisplayProperties()const;
		std::vector< VkDisplayPlanePropertiesKHR > getDisplayPlaneProperties()const;
		std::vector< VkDisplayKHR > getDisplayPlaneSupportedDisplays( uint32_t planeIndex )const;
#endif

		VkInstance getInstance()const noexcept
		{
			return m_instance;
		}

	private:
		void doInitialise();

	private:
		VkInstance m_instance;
		VkPhysicalDeviceFeatures m_features{};
		VkPhysicalDeviceProperties m_properties{};
		VkQueueFamilyPropertiesArray m_queueProperties{};
		mutable std::map< VkFormat, VkFormatProperties > m_formatProperties;
#if VK_KHR_portability_subset
		VkPhysicalDevicePortabilitySubsetFeaturesKHR m_portabilityFeatures{};
#endif
#if VK_VERSION_1_1
		VkPhysicalDeviceFeatures2 m_features2{};
		VkPhysicalDeviceProperties2 m_properties2{};
		std::vector< VkQueueFamilyProperties2 > m_queueProperties2{};
		mutable std::map< VkFormat, VkFormatProperties2 > m_formatProperties2;
#elif VK_KHR_get_physical_device_properties2
		VkPhysicalDeviceFeatures2KHR m_features2{};
		VkPhysicalDeviceProperties2KHR m_properties2{};
		std::vector< VkQueueFamilyProperties2KHR > m_queueProperties2{};
		mutable std::map< VkFormat, VkFormatProperties2KHR > m_formatProperties2;
#endif
#ifdef VK_KHR_display
		std::vector< std::string > m_displayNames;
		std::vector< VkDisplayPropertiesKHR >m_displays;
		std::vector< VkDisplayPlanePropertiesKHR > m_displayPlanes;
#endif
	};
}
