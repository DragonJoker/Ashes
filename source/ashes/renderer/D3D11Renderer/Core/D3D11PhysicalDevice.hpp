/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/D3D11Renderer/D3D11RendererPrerequisites.hpp"

#include <renderer/RendererCommon/IcdObject.hpp>

#include <map>
#include <vector>

struct AGSContext;

namespace ashes::d3d11
{
	VkImageUsageFlags getUsageFlags( VkFormatFeatureFlags flags );

	class PhysicalDevice
		: public ashes::IcdObject
		, public NonCopyable
	{
	public:
		PhysicalDevice( VkInstance instance
			, AdapterInfo adapterInfo );
		~PhysicalDevice()noexcept;

		uint32_t getMemoryTypeBits( VkMemoryPropertyFlags properties )const;
		uint32_t getMemoryTypeBits( VkMemoryPropertyFlags properties1
			, VkMemoryPropertyFlags properties2 )const;
		VkBool32 getPresentationSupport()const;
		VkLayerPropertiesArray const & enumerateLayerProperties()const;
		VkExtensionPropertiesArray const & enumerateExtensionProperties()const;
		VkPhysicalDeviceProperties const & getProperties()const;
		VkPhysicalDeviceMemoryProperties const & getMemoryProperties()const;
		VkPhysicalDeviceFeatures const & getFeatures()const;
		VkQueueFamilyPropertiesArray const & getQueueFamilyProperties()const;
		VkFormatProperties const & getFormatProperties( VkFormat fmt )const;
		VkResult getImageFormatProperties( VkFormat format
			, VkImageType type
			, VkImageTiling tiling
			, VkImageUsageFlags usage
			, VkImageCreateFlags flags
			, VkImageFormatProperties & imageProperties )const;
		VkResult getSparseImageFormatProperties( VkFormat format
			, VkImageType type
			, VkSampleCountFlagBits samples
			, VkImageUsageFlags usage
			, VkImageTiling tiling
			, std::vector< VkSparseImageFormatProperties > & sparseImageFormatProperties )const;

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

		inline IDXGIAdapter * getAdapter()const
		{
			return m_adapterInfo.adapter;
		}

		inline IDXGIAdapter1 * getAdapter1()const
		{
			return m_adapterInfo.adapter1;
		}

		inline IDXGIAdapter2 * getAdapter2()const
		{
			return m_adapterInfo.adapter2;
		}

		inline IDXGIOutput * getOutput()const
		{
			return m_adapterInfo.output;
		}

		inline D3D_FEATURE_LEVEL getFeatureLevel()const
		{
			return m_adapterInfo.featureLevel;
		}

		inline VkInstance getInstance()const
		{
			return m_instance;
		}

		inline bool isNVIDIA()const
		{
			return m_properties.vendorID == 0x10DE;
		}

		inline bool isAMD()const
		{
			return m_properties.vendorID == 0x1002;
		}

		inline AGSContext * getAGSContext()const
		{
			assert( isAMD() );
			return m_AGSContext;
		}

	private:
		void doInitialise();
		void doInitialiseProperties();
		void doInitialiseFeatures();
		void doInitialiseQueueProperties();
		void doInitialiseFormatProperties();
		void doInitialisePortability();
#ifdef VK_KHR_display
		void doInitialiseDisplayProperties();
		void doInitialiseDisplayPlaneProperties();
#endif

	private:
		VkInstance m_instance;
		AdapterInfo m_adapterInfo;
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
#else
#	if VK_KHR_get_physical_device_properties2
		VkPhysicalDeviceFeatures2KHR m_features2{};
		VkPhysicalDeviceProperties2KHR m_properties2{};
		std::vector< VkQueueFamilyProperties2KHR > m_queueProperties2{};
		mutable std::map< VkFormat, VkFormatProperties2KHR > m_formatProperties2;
#	endif
#endif
#ifdef VK_KHR_display
		std::vector< VkDisplayPropertiesKHR > m_displays;
		std::vector< VkDisplayPlanePropertiesKHR > m_displayPlanes;
#endif
		AGSContext * m_AGSContext{ nullptr };
	};
}
