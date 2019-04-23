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
		*\~english
		*\brief
		*	Constructor.
		*\param[in] instance
		*	The renderer instance.
		*\~french
		*\brief
		*	Constructeur.
		*\param[in] instance
		*	L'instance de rendu.
		*/
		PhysicalDevice( Instance const & instance
			, VkPhysicalDevice gpu );
		/**
		*\~english
		*\brief
		*	Destructor.
		*\~french
		*\brief
		*	Destructeur.
		*/
		~PhysicalDevice() = default;
		/**
		*\~french
		*\brief
		*	Enumère les couches du périphérique.
		*\~english
		*\brief
		*	Enumerates the devie layers.
		*/
		VkLayerPropertiesArray enumerateLayerProperties()const;
		/**
		*\~french
		*\brief
		*	Enumère les extensions de la couche de périphérique donnée.
		*\~english
		*\brief
		*	Enumerates the extension for the given device layer.
		*/
		VkExtensionPropertiesArray enumerateExtensionProperties( std::string const & layerName )const;
		/**
		*\~english
		*\brief
		*	Retrieves the physical device properties.
		*\~french
		*\brief
		*	Récupère les propriétés du périphérique physique.
		*/
		VkPhysicalDeviceProperties getProperties()const;
		/**
		*\~english
		*\brief
		*	Retrieves the physical device memory properties.
		*\~french
		*\brief
		*	Récupère les propriétés mémoire du périphérique physique.
		*/
		VkPhysicalDeviceMemoryProperties getMemoryProperties()const;
		/**
		*\~english
		*\brief
		*	Retrieves the physical device features.
		*\~french
		*\brief
		*	Récupère les fonctionnalités du périphérique physique.
		*/
		VkPhysicalDeviceFeatures getFeatures()const;
		/**
		*\~english
		*\brief
		*	Retrieves the physical device queues properties.
		*\~french
		*\brief
		*	Récupère les propriétés des files du périphérique physique.
		*/
		VkQueueFamilyPropertiesArray getQueueFamilyProperties()const;
		/**
		*\~english
		*\brief
		*	Retrieves the given format's properties.
		*\~french
		*\brief
		*	Récupère les propriétés du format donné.
		*/
		VkFormatProperties getFormatProperties( VkFormat fmt )const;
		/**
		*\~english
		*\brief
		*	Dumps the properties and features to a std::string.
		*\~french
		*\brief
		*	Dumpe les propriétés et fonctionnalités dans un std::string.
		*/ 
		std::string dumpProperties()const;
		/**
		*\~english
		*name
		*	Getters.
		*\~french
		*name
		*	Accesseurs.
		*/
		/**@{*/
		inline uint32_t getShaderVersion()const
		{
			return m_shaderVersion;
		}
		/**@}*/
		/**
		*\~french
		*\brief
		*	Conversion implicite vers VkPhysicalDevice.
		*\~english
		*\brief
		*	VkPhysicalDevice implicit cast operator.
		*/
		inline operator VkPhysicalDevice const & ()const
		{
			return m_gpu;
		}

	private:
		Instance const & m_instance;
		uint32_t m_shaderVersion;
		VkPhysicalDevice m_gpu{ VK_NULL_HANDLE };
	};
}

#endif
