/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "Gl3Renderer/GlRendererPrerequisites.hpp"

#include <Ashes/Core/PhysicalDevice.hpp>

namespace gl_renderer
{
	/**
	*\~french
	*\brief
	*	Classe contenant les informations liées au GPU physique.
	*\~english
	*\brief
	*	Wraps informations about physical GPU.
	*/
	class PhysicalDevice
		: public ashes::PhysicalDevice
	{
	public:
		PhysicalDevice( Instance const & instance );
		/**
		*\copydoc	ashes::Instance::enumerateLayerProperties
		*/
		ashes::VkLayerPropertiesArray enumerateLayerProperties()const override;
		/**
		*\copydoc	ashes::Instance::enumerateExtensionProperties
		*/
		ashes::VkExtensionPropertiesArray enumerateExtensionProperties( std::string const & layerName )const override;
		/**
		*\copydoc	ashes::Instance::getProperties
		*/
		ashes::PhysicalDeviceProperties getProperties()const override;
		/**
		*\copydoc	ashes::Instance::getMemoryProperties
		*/
		ashes::PhysicalDeviceMemoryProperties getMemoryProperties()const override;
		/**
		*\copydoc	ashes::Instance::getFeatures
		*/
		ashes::PhysicalDeviceFeatures getFeatures()const override;
		/**
		*\copydoc	ashes::Instance::getQueueFamilyProperties
		*/
		ashes::QueueFamilyPropertiesArray getQueueFamilyProperties()const override;
		/**
		*\copydoc	ashes::Instance::getFormatProperties
		*/
		ashes::FormatProperties getFormatProperties( VkFormat fmt )const override;

		inline Instance const & getInstance()const
		{
			return m_instance;
		}

		bool find( std::string const & name )const;
		bool findAny( ashes::StringArray const & names )const;
		bool findAll( ashes::StringArray const & names )const;

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
		Instance const & m_instance;
		ashes::PhysicalDeviceFeatures m_features{};
		ashes::PhysicalDeviceProperties m_properties{};
		ashes::QueueFamilyPropertiesArray m_queueProperties{};
		mutable std::map< VkFormat, ashes::FormatProperties > m_formatProperties;
	};
}
