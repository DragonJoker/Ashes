/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/TestRenderer/TestRendererPrerequisites.hpp"

namespace ashes::test
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
		PhysicalDevice( VkInstance instance );
		~PhysicalDevice();
		/**
		*\copydoc	ashes::Instance::getPresentationSupport
		*/
		VkBool32 getPresentationSupport( uint32_t queueFamilyIndex )const;
		/**
		*\copydoc	ashes::Instance::enumerateLayerProperties
		*/
		VkLayerPropertiesArray enumerateLayerProperties()const;
		/**
		*\copydoc	ashes::Instance::enumerateExtensionProperties
		*/
		VkExtensionPropertiesArray enumerateExtensionProperties( std::string const & layerName )const;
		/**
		*\copydoc	ashes::Instance::getProperties
		*/
		VkPhysicalDeviceProperties const & getProperties()const;
		/**
		*\copydoc	ashes::Instance::getMemoryProperties
		*/
		VkPhysicalDeviceMemoryProperties getMemoryProperties()const;
		/**
		*\copydoc	ashes::Instance::getFeatures
		*/
		VkPhysicalDeviceFeatures getFeatures()const;
		/**
		*\copydoc	ashes::Instance::getQueueFamilyProperties
		*/
		VkQueueFamilyPropertiesArray getQueueFamilyProperties()const;
		/**
		*\copydoc	ashes::Instance::getFormatProperties
		*/
		VkFormatProperties getFormatProperties( VkFormat fmt )const;

		inline VkInstance getInstance()const
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
	};
}
