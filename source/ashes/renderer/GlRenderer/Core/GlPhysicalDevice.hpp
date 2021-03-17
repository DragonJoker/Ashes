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

#ifdef VK_VERSION_1_2
		VkPhysicalDeviceDriverProperties const & getDriverProperties()const;
#elif VK_KHR_driver_properties
		VkPhysicalDeviceDriverPropertiesKHR const & getDriverProperties()const;
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
#if VK_VERSION_1_2
		VkPhysicalDeviceDriverProperties m_driverProperties{};
#elif VK_KHR_driver_properties
		VkPhysicalDeviceDriverPropertiesKHR m_driverProperties{};
#endif
#if VK_KHR_portability_subset
		VkPhysicalDevicePortabilitySubsetFeaturesKHR m_portabilityFeatures{};
#endif
#if VK_VERSION_1_1
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
