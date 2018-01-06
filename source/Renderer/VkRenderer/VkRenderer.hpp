/**
*\file
*	Renderer.h
*\author
*	Sylvain Doremus
*/
#pragma once

#include "VkRendererPrerequisites.hpp"

#include <VkLib/Instance.hpp>
#include <VkLib/VulkanLibrary.hpp>
#include <Renderer/Renderer.hpp>

namespace vk_renderer
{
	class Renderer
		: public renderer::Renderer
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
		renderer::DevicePtr createDevice( renderer::ConnectionPtr && connection )const override;
		/**
		*\brief
		*	Constructeur.
		*\param[in] renderer
		*	L'instance de Renderer.
		*\param[in] deviceIndex
		*	L'indice du périphérique physique.
		*\param[in] handle
		*	Le descripteur de la fenêtre.
		*/
		renderer::ConnectionPtr createConnection( uint32_t deviceIndex
			, renderer::WindowHandle && handle )const override;
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
