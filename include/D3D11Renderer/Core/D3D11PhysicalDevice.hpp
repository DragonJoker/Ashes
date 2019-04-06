/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "D3D11Renderer/D3D11RendererPrerequisites.hpp"

#include <Ashes/Core/PhysicalDevice.hpp>

namespace d3d11_renderer
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
		PhysicalDevice( Instance const & instance
			, AdapterInfo adapterInfo );
		~PhysicalDevice();
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
			return m_output;
		}

		inline D3D_FEATURE_LEVEL getFeatureLevel()const
		{
			return m_adapterInfo.featureLevel;
		}

	private:
		void doInitialise();

	private:
		Instance const & m_instance;
		AdapterInfo m_adapterInfo;
		IDXGIOutput * m_output;
		ashes::PhysicalDeviceFeatures m_features{};
		ashes::PhysicalDeviceProperties m_properties{};
		ashes::QueueFamilyPropertiesArray m_queueProperties{};
		mutable std::map< VkFormat, ashes::FormatProperties > m_formatProperties;
	};
}
