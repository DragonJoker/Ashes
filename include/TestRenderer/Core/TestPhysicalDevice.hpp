/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "TestRendererPrerequisites.hpp"

#include <Ashes/Core/PhysicalDevice.hpp>

namespace test_renderer
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
		/**
		*\~french
		*\brief
		*	Constructeur.
		*\param[in] vulkan
		*	L'instance Vulkan.
		*\param[in] gpu
		*	Le GPU physique.
		*\~english
		*\brief
		*	Constructor.
		*\param[in] vulkan
		*	The Vulkan instance.
		*\param[in] gpu
		*	The Vulkan physical GPU.
		*/
		PhysicalDevice( Instance const & instance );
		/**
		*\copydoc	ashes::Instance::enumerateLayerProperties
		*/
		ashes::LayerPropertiesArray enumerateLayerProperties()const override;
		/**
		*\copydoc	ashes::Instance::enumerateExtensionProperties
		*/
		ashes::ExtensionPropertiesArray enumerateExtensionProperties( std::string const & layerName )const override;
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
		ashes::FormatProperties getFormatProperties( ashes::Format fmt )const override;

	private:
		void doInitialise();

	private:
		Instance const & m_instance;
		ashes::PhysicalDeviceFeatures m_features{};
		ashes::PhysicalDeviceProperties m_properties{};
		ashes::QueueFamilyPropertiesArray m_queueProperties{};
		std::array< ashes::FormatProperties, size_t( ashes::Format::eRange ) > m_formatProperties;
	};
}
