/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "Gl4Renderer/GlRendererPrerequisites.hpp"

namespace ashes::gl4
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
	{
	public:
		/**
		*\~french
		*\brief
		*	Constructeur.
		*\param[in] renderer
		*	L'instance de rendu.
		*\~english
		*\brief
		*	Constructor.
		*\param[in] renderer
		*	The rendering instance.
		*/
		PhysicalDevice( VkInstance instance );
		inline VkBool32 getPresentationSupport( uint32_t queueFamilyIndex )const
		{
			return true;
		}
		/**
		*\copydoc	Instance::enumerateLayerProperties
		*/
		std::vector< VkLayerProperties > enumerateLayerProperties()const;
		/**
		*\copydoc	Instance::enumerateExtensionProperties
		*/
		std::vector< VkExtensionProperties > enumerateExtensionProperties( std::string const & layerName )const;
		/**
		*\copydoc	Instance::getProperties
		*/
		VkPhysicalDeviceProperties const & getProperties()const;
		/**
		*\copydoc	Instance::getMemoryProperties
		*/
		VkPhysicalDeviceMemoryProperties const & getMemoryProperties()const;
		/**
		*\copydoc	Instance::getFeatures
		*/
		VkPhysicalDeviceFeatures const & getFeatures()const;
		/**
		*\copydoc	Instance::getQueueFamilyProperties
		*/
		std::vector< VkQueueFamilyProperties > const & getQueueFamilyProperties()const;
		/**
		*\copydoc	Instance::getFormatProperties
		*/
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
