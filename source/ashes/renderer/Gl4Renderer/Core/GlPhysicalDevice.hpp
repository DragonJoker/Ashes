/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/Gl4Renderer/GlRendererPrerequisites.hpp"

namespace ashes::gl4
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
	};
}
