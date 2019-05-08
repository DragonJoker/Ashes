/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/D3D11Renderer/D3D11RendererPrerequisites.hpp"

namespace ashes::d3d11
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
		PhysicalDevice( VkInstance instance
			, AdapterInfo adapterInfo );
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
		VkPhysicalDeviceProperties getProperties()const;
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

	private:
		void doInitialise();

	private:
		VkInstance m_instance;
		AdapterInfo m_adapterInfo;
		VkPhysicalDeviceFeatures m_features{};
		VkPhysicalDeviceProperties m_properties{};
		VkQueueFamilyPropertiesArray m_queueProperties{};
		mutable std::map< VkFormat, VkFormatProperties > m_formatProperties;
	};
}
