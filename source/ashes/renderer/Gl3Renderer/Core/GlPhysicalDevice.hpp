/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/Gl3Renderer/GlRendererPrerequisites.hpp"

namespace ashes::gl3
{
	class PhysicalDevice
	{
	public:
		PhysicalDevice( VkInstance instance );

		inline VkBool32 getPresentationSupport( uint32_t queueFamilyIndex )const
		{
			return true;
		}

		std::vector< VkLayerProperties > enumerateLayerProperties()const;
		std::vector< VkExtensionProperties > enumerateExtensionProperties( std::string const & layerName )const;
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

		bool find( std::string const & name )const;
		bool findAny( StringArray const & names )const;
		bool findAll( StringArray const & names )const;
		int getMajor()const;
		int getMinor()const;

		inline VkInstance getInstance()const
		{
			return m_instance;
		}

	private:
		void doInitialise();
		void doGetValue( GLenum name, int32_t & value )const;
		void doGetValue( GLenum name, uint32_t & value )const;
		void doGetValues( GLenum name, int32_t( &value )[2] )const;
		void doGetValues( GLenum name, uint32_t( &value )[2] )const;
		void doGetValues( GLenum name, int32_t( &value )[3] )const;
		void doGetValues( GLenum name, uint32_t( &value )[3] )const;
		void doGetValuesI( GLenum name, int32_t( &value )[2] )const;
		void doGetValuesI( GLenum name, uint32_t( &value )[2] )const;
		void doGetValuesI( GLenum name, int32_t( &value )[3] )const;
		void doGetValuesI( GLenum name, uint32_t( &value )[3] )const;
		void doGetValue( GLenum name, int64_t & value )const;
		void doGetValue( GLenum name, uint64_t & value )const;
		void doGetValues( GLenum name, int64_t( &value )[2] )const;
		void doGetValues( GLenum name, uint64_t( &value )[2] )const;
		void doGetValues( GLenum name, int64_t( &value )[3] )const;
		void doGetValues( GLenum name, uint64_t( &value )[3] )const;
		void doGetValuesI( GLenum name, int64_t( &value )[2] )const;
		void doGetValuesI( GLenum name, uint64_t( &value )[2] )const;
		void doGetValuesI( GLenum name, int64_t( &value )[3] )const;
		void doGetValuesI( GLenum name, uint64_t( &value )[3] )const;
		void doGetValue( GLenum name, float & value )const;
		void doGetValues( GLenum name, float( &value )[2] )const;
		void doGetValues( GLenum name, float( &value )[3] )const;
		void doGetValuesI( GLenum name, float( &value )[2] )const;
		void doGetValuesI( GLenum name, float( &value )[3] )const;

	private:
		VkInstance m_instance;
		VkPhysicalDeviceFeatures m_features{};
		VkPhysicalDeviceProperties m_properties{};
		std::vector< VkQueueFamilyProperties > m_queueProperties{};
		mutable std::map< VkFormat, VkFormatProperties > m_formatProperties;
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
	};
}
