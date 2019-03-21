/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#ifndef ___Ashes_PhysicalDevice_HPP___
#define ___Ashes_PhysicalDevice_HPP___
#pragma once

#include "Ashes/Miscellaneous/ExtensionProperties.hpp"
#include "Ashes/Miscellaneous/FormatProperties.hpp"
#include "Ashes/Miscellaneous/LayerProperties.hpp"
#include "Ashes/Miscellaneous/PhysicalDeviceFeatures.hpp"
#include "Ashes/Miscellaneous/PhysicalDeviceMemoryProperties.hpp"
#include "Ashes/Miscellaneous/PhysicalDeviceProperties.hpp"
#include "Ashes/Miscellaneous/QueueFamilyProperties.hpp"

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
		PhysicalDevice( Instance const & instance );
		/**
		*\~english
		*\brief
		*	Destructor.
		*\~french
		*\brief
		*	Destructeur.
		*/
		virtual ~PhysicalDevice() = default;
		/**
		*\~french
		*\brief
		*	Enumère les couches du périphérique.
		*\~english
		*\brief
		*	Enumerates the devie layers.
		*/
		virtual ashes::LayerPropertiesArray enumerateLayerProperties()const = 0;
		/**
		*\~french
		*\brief
		*	Enumère les extensions de la couche de périphérique donnée.
		*\~english
		*\brief
		*	Enumerates the extension for the given device layer.
		*/
		virtual ashes::ExtensionPropertiesArray enumerateExtensionProperties( std::string const & layerName )const = 0;
		/**
		*\~english
		*\brief
		*	Retrieves the physical device properties.
		*\~french
		*\brief
		*	Récupère les propriétés du périphérique physique.
		*/
		virtual PhysicalDeviceProperties getProperties()const = 0;
		/**
		*\~english
		*\brief
		*	Retrieves the physical device memory properties.
		*\~french
		*\brief
		*	Récupère les propriétés mémoire du périphérique physique.
		*/
		virtual PhysicalDeviceMemoryProperties getMemoryProperties()const = 0;
		/**
		*\~english
		*\brief
		*	Retrieves the physical device features.
		*\~french
		*\brief
		*	Récupère les fonctionnalités du périphérique physique.
		*/
		virtual PhysicalDeviceFeatures getFeatures()const = 0;
		/**
		*\~english
		*\brief
		*	Retrieves the physical device queues properties.
		*\~french
		*\brief
		*	Récupère les propriétés des files du périphérique physique.
		*/
		virtual QueueFamilyPropertiesArray getQueueFamilyProperties()const = 0;
		/**
		*\~english
		*\brief
		*	Retrieves the given format's properties.
		*\~french
		*\brief
		*	Récupère les propriétés du format donné.
		*/
		virtual FormatProperties getFormatProperties( Format fmt )const = 0;
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

	protected:
		uint32_t m_shaderVersion;

	private:
		Instance const & m_instance;
	};
}

#endif
