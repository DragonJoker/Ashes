/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#ifndef ___AshesPP_PhysicalDevice_HPP___
#define ___AshesPP_PhysicalDevice_HPP___
#pragma once

#include "ashespp/AshesPPPrerequisites.hpp"

namespace ashes
{
	/**
	*\brief
	*	Wraps informations about physical GPU.
	*/
	class PhysicalDevice
	{
	public:
		/**
		*\brief
		*	Constructor.
		*\param[in] instance
		*	The renderer instance.
		*\param[in] gpu
		*	The Vulkan physical device.
		*/
		PhysicalDevice( Instance const & instance
			, VkPhysicalDevice gpu );
		/**
		*\brief
		*	Destructor.
		*/
		~PhysicalDevice() = default;
		/**
		*\brief
		*	Enumerates the devie layers.
		*/
		VkLayerPropertiesArray enumerateLayerProperties()const;
		/**
		*\brief
		*	Enumerates the extension for the given device layer.
		*/
		VkExtensionPropertiesArray enumerateExtensionProperties( std::string const & layerName )const;
		/**
		*\brief
		*	Retrieves the physical device properties.
		*/
		VkPhysicalDeviceProperties getProperties()const;
		/**
		*\brief
		*	Retrieves the physical device memory properties.
		*/
		VkPhysicalDeviceMemoryProperties getMemoryProperties()const;
		/**
		*\brief
		*	Retrieves the physical device features.
		*/
		VkPhysicalDeviceFeatures getFeatures()const;
		/**
		*\brief
		*	Retrieves the physical device features.
		*/
		void getFeatures( VkPhysicalDeviceFeatures2 & features )const;
		/**
		*\brief
		*	Retrieves the physical device queues properties.
		*/
		VkQueueFamilyPropertiesArray getQueueFamilyProperties()const;
		/**
		*\brief
		*	Retrieves the given format's properties.
		*/
		VkFormatProperties getFormatProperties( VkFormat fmt )const;
		/**
		*\brief
		*	Retrieves the image format properties.
		*/
		VkResult getImageFormatProperties( VkFormat format
			, VkImageType type
			, VkImageTiling tiling
			, VkImageUsageFlags usage
			, VkImageCreateFlags flags
			, VkImageFormatProperties & imageProperties )const;
		/**
		*\brief
		*	Dumps the properties and features to a std::string.
		*/ 
		std::string dumpProperties()const;
		/**
		*\brief
		*	VkPhysicalDevice implicit cast operator.
		*/
		inline operator VkPhysicalDevice const & ()const
		{
			return m_gpu;
		}

	private:
		Instance const & m_instance;
		VkPhysicalDevice m_gpu{};
	};
}

#endif
