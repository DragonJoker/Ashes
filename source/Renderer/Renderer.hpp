/**
*\file
*	Renderer.h
*\author
*	Sylvain Doremus
*/
#pragma once

#include "RendererPrerequisites.hpp"

#include <VkLib/Instance.hpp>
#include <VkLib/VulkanLibrary.hpp>

namespace renderer
{
	class Renderer
	{
	public:
		/**
		*\brief
		*	Constructeur, initialise l'instance de Vulkan.
		*/
		Renderer();
		/**
		*\brief
		*	Destructeur.
		*/
		~Renderer();
		/**
		*\brief
		*	Crée le périphérique logique.
		*\param[in] connection
		*	La connection avec la fenêtre.
		*/
		DevicePtr createDevice( Connection & connection );
		/**
		*\return
		*	L'instance de vulkan.
		*/
		inline vk::Instance const & getInstance()const
		{
			return m_vulkan;
		}
		/**
		*\brief
		*	Récupère le GPU physique à l'index donné.
		*\param[in] index
		*	L'index.
		*\return
		*	Le GPU physique.
		*/
		inline vk::PhysicalDevice const & getPhysicalDevice( uint32_t index = 0u )const
		{
			return m_vulkan.getPhysicalDevice( index );
		}

	private:
		vk::VulkanLibrary m_library;
		vk::Instance m_vulkan;
	};
}
