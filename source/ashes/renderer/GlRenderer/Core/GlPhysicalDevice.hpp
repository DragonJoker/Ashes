/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/GlRenderer/GlRendererPrerequisites.hpp"

namespace ashes::gl
{
	class PhysicalDevice
		: public ashes::IcdObject
	{
	public:
		PhysicalDevice( VkInstance instance );

		VkBool32 getPresentationSupport( uint32_t queueFamilyIndex )const
		{
			return true;
		}

		uint32_t getMemoryTypeBits( VkMemoryPropertyFlags properties )const;
		uint32_t getMemoryTypeBits( VkMemoryPropertyFlags properties1
			, VkMemoryPropertyFlags properties2 )const;
		std::vector< VkLayerProperties > enumerateLayerProperties()const;
		std::vector< VkExtensionProperties > enumerateExtensionProperties( const char * layerName )const;
		VkPhysicalDeviceProperties const & getProperties()const;
		VkPhysicalDeviceMemoryProperties const & getMemoryProperties()const;
		VkPhysicalDeviceFeatures const & getFeatures()const;
		std::vector< VkQueueFamilyProperties > const & getQueueFamilyProperties()const;
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
		VkPhysicalDeviceDriverProperties const & getDriverProperties()const;
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
		VkPhysicalDeviceDriverPropertiesKHR const & getDriverProperties()const;
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
#if VK_KHR_portability_subset
		VkPhysicalDevicePortabilitySubsetFeaturesKHR getPortabilitySubset()const
		{
			return m_portabilityFeatures;
		}
#endif

		bool find( VkExtensionProperties const & name )const;
		bool findAny( VkExtensionPropertiesArray const & names )const;
		bool findAll( VkExtensionPropertiesArray const & names )const;
		int getMajor()const;
		int getMinor()const;

		VkInstance getInstance()const
		{
			return m_instance;
		}
		
		GlPhysicalDeviceFeatures const & getGlFeatures()const
		{
			return m_glFeatures;
		}

	private:
		void doInitialise();
		void doInitialiseProperties( ContextLock & context );
		void doInitialiseFeatures( ContextLock & context );
		void doInitialiseMemoryProperties( ContextLock & context );
		void doInitialiseQueueProperties( ContextLock & context );
		void doInitialiseDisplayProperties( ContextLock & context );
		void doInitialiseDriverProperties( ContextLock & context );
		void doInitialiseMemoryProperties2( ContextLock & context );
		void doInitialiseProperties2( ContextLock & context );
		void doInitialisePortability( ContextLock & context );

	private:
		VkInstance m_instance;
		VkPhysicalDeviceFeatures m_features{};
		VkPhysicalDeviceMemoryProperties m_memoryProperties{};
		VkPhysicalDeviceProperties m_properties{};
		GlPhysicalDeviceFeatures m_glFeatures{};
		std::vector< VkQueueFamilyProperties > m_queueProperties{};
		mutable std::map< VkFormat, VkFormatProperties > m_formatProperties;
		mutable std::map< size_t, std::pair< VkImageFormatProperties, VkResult > > m_imageFormatProperties;
#if VK_KHR_portability_subset
		VkPhysicalDevicePortabilitySubsetFeaturesKHR m_portabilityFeatures{};
#endif
#if VK_VERSION_1_1
		VkPhysicalDeviceFeatures2 m_features2{};
		VkPhysicalDeviceMemoryProperties2 m_memoryProperties2{};
		VkPhysicalDeviceProperties2 m_properties2{};
		VkPhysicalDeviceDriverProperties m_driverProperties{};
		std::vector< VkQueueFamilyProperties2 > m_queueProperties2{};
		mutable std::map< VkFormat, VkFormatProperties2 > m_formatProperties2;
#elif VK_KHR_get_physical_device_properties2
		VkPhysicalDeviceFeatures2KHR m_features2{};
		VkPhysicalDeviceMemoryProperties2KHR m_memoryProperties2{};
		VkPhysicalDeviceProperties2KHR m_properties2{};
		VkPhysicalDeviceDriverPropertiesKHR m_driverProperties{};
		std::vector< VkQueueFamilyProperties2KHR > m_queueProperties2{};
		mutable std::map< VkFormat, VkFormatProperties2KHR > m_formatProperties2;
#endif
#ifdef VK_KHR_display
		std::vector< std::string > m_displayNames;
		std::vector< VkDisplayPropertiesKHR >m_displays;
		std::vector< VkDisplayPlanePropertiesKHR > m_displayPlanes;
#endif
	};

	bool has420PackExtensions( VkPhysicalDevice physicalDevice );
	bool hasCopyImage( VkPhysicalDevice physicalDevice );
	bool hasProgramPipelines( VkPhysicalDevice physicalDevice );
	bool hasSamplerAnisotropy( VkPhysicalDevice physicalDevice );
	bool hasTextureStorage( VkPhysicalDevice physicalDevice );
	bool hasTextureViews( VkPhysicalDevice physicalDevice );
	bool hasViewportArrays( VkPhysicalDevice physicalDevice );
	bool hasProgramInterfaceQuery( VkPhysicalDevice physicalDevice );
}
