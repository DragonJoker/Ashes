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
		PhysicalDevice( Instance & instance
			, VkPhysicalDevice gpu );
		/**
		*\copydoc	ashes::PhysicalDevice::initialise
		*/
		void initialise()override;
		/**
		*\~french
		*\return
		*	Les noms des extensions supportées par le GPU.
		*\~english
		*\return
		*	The GPU's supported extensions names.
		*/
		inline ashes::StringArray const & getExtensionNames()const
		{
			return m_extensionNames;
		}
		/**
		*\~french
		*\return
		*	Les noms des couches supportées par le GPU.
		*\~english
		*\return
		*	The GPU's supported layers names.
		*/
		inline ashes::StringArray const & getLayerNames()const
		{
			return m_layerNames;
		}
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
		Instance & m_instance;
		VkPhysicalDevice m_gpu{ VK_NULL_HANDLE };
		ashes::StringArray m_extensionNames;
		ashes::StringArray m_layerNames;
	};
}
