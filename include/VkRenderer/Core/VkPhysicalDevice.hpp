/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "VkRenderer/VkRendererPrerequisites.hpp"

#include <Ashes/Core/PhysicalDevice.hpp>

namespace vk_renderer
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
		PhysicalDevice( Instance const & instance
			, VkPhysicalDevice gpu );
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
		/**
		*\~french
		*\brief
		*	Conversion implicite vers VkPhysicalDevice.
		*\~english
		*\brief
		*	VkPhysicalDevice implicit cast operator.
		*/
		inline operator VkPhysicalDevice const &()const
		{
			return m_gpu;
		}

	private:
		Instance const & m_instance;
		VkPhysicalDevice m_gpu{ VK_NULL_HANDLE };
	};
}
