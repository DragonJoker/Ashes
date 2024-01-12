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
		explicit PhysicalDevice( VkInstance instance );

		VkBool32 getPresentationSupport( [[maybe_unused]] uint32_t queueFamilyIndex )const noexcept
		{
			return true;
		}

		uint32_t getMemoryTypeBits( VkMemoryPropertyFlags properties )const noexcept;
		uint32_t getMemoryTypeBits( VkMemoryPropertyFlags properties1
			, VkMemoryPropertyFlags properties2 )const noexcept;
		std::vector< VkLayerProperties > const & enumerateLayerProperties()const noexcept;
		std::vector< VkExtensionProperties > const & enumerateExtensionProperties( const char * layerName )const noexcept;
		VkPhysicalDeviceProperties const & getProperties()const noexcept;
		VkPhysicalDeviceMemoryProperties const & getMemoryProperties()const noexcept;
		VkPhysicalDeviceFeatures const & getFeatures()const noexcept;
		std::vector< VkQueueFamilyProperties > const & getQueueFamilyProperties()const noexcept;
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
		VkPhysicalDevicePortabilitySubsetFeaturesKHR getPortabilitySubset()const noexcept
		{
			return m_portabilityFeatures;
		}
#endif
#if VK_EXT_inline_uniform_block
		VkPhysicalDeviceInlineUniformBlockFeaturesEXT getInlineUniformBlockFeatures()const noexcept
		{
			return m_inlineUniformBlockFeatures;
		}
		VkPhysicalDeviceInlineUniformBlockPropertiesEXT getInlineUniformBlockProperties()const noexcept
		{
			return m_inlineUniformBlockProperties;
		}
#endif

		bool find( VkExtensionProperties const & name )const;
		bool findAny( VkExtensionPropertiesArray const & names )const;
		bool findAll( VkExtensionPropertiesArray const & names )const;
		int getMajor()const;
		int getMinor()const;

		VkInstance getInstance()const noexcept
		{
			return m_instance;
		}
		
		GlPhysicalDeviceFeatures const & getGlFeatures()const noexcept
		{
			return m_glFeatures;
		}

	private:
		void doInitialise();
		void doInitialiseProperties( ContextLock const & context );
		void doInitialiseFeatures( ContextLock const & context );
		void doInitialiseMemoryProperties( ContextLock const & context );
		void doInitialiseQueueProperties();
		void doInitialiseDisplayProperties();
		void doInitialiseDriverProperties();
		void doInitialisePortability();
		void doInitialiseInlineUniformBlock( ContextLock const & context );

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
#if VK_EXT_inline_uniform_block
		VkPhysicalDeviceInlineUniformBlockFeaturesEXT m_inlineUniformBlockFeatures{};
		VkPhysicalDeviceInlineUniformBlockPropertiesEXT m_inlineUniformBlockProperties{};
#endif
#ifdef VK_KHR_display
		std::vector< std::string > m_displayNames;
		std::vector< VkDisplayPropertiesKHR >m_displays;
		std::vector< VkDisplayPlanePropertiesKHR > m_displayPlanes;
#endif
	};

	bool has420PackExtensions( VkPhysicalDevice physicalDevice )noexcept;
	bool hasCopyImage( VkPhysicalDevice physicalDevice )noexcept;
	bool hasProgramPipelines( VkPhysicalDevice physicalDevice )noexcept;
	bool hasSamplerAnisotropy( VkPhysicalDevice physicalDevice )noexcept;
	bool hasTextureStorage( VkPhysicalDevice physicalDevice )noexcept;
	bool hasTextureViews( VkPhysicalDevice physicalDevice )noexcept;
	bool hasViewportArrays( VkPhysicalDevice physicalDevice )noexcept;
	bool hasProgramInterfaceQuery( VkPhysicalDevice physicalDevice )noexcept;
}
